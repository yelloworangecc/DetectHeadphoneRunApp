#include <iostream>
#include "MMNotificationClientImpl.h"
using namespace std;

#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }


bool MMNotificationClientImpl::print_switch = false;

void MMNotificationClientImpl::setPrintSwitch(bool flag)
{
  print_switch = flag;
  return;
}

MMNotificationClientImpl::MMNotificationClientImpl():
  pEnumerator(NULL),ref(1)
{
  CoInitialize(NULL);
  HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),
				NULL, CLSCTX_INPROC_SERVER,
				__uuidof(IMMDeviceEnumerator),
				(void**)&pEnumerator);
  if (S_OK!=hr){pEnumerator=NULL;} 
  resetFlags();
}

MMNotificationClientImpl::~MMNotificationClientImpl()
{
  SAFE_RELEASE(pEnumerator)
  CoUninitialize();
}


HRESULT STDMETHODCALLTYPE MMNotificationClientImpl::OnDefaultDeviceChanged(EDataFlow flow, //eRender, eCapture
									   ERole role,//eConsole, eMultimedia, eCommunications
									   LPCWSTR pwstrDeviceId)
{
  if (print_switch && pEnumerator!=NULL)
  {
    cout << "--------DefaultDeviceChanged--------"<< endl;
	
    switch (flow){
    case eRender: cout << "eRender "; break;
    case eCapture: cout << "eCapture "; break;
    default: cout << "UNKNOWN ";
    }
    switch (role) {
    case eConsole: cout << "eConsole"; break;
    case eMultimedia: cout << "eMultimedia"; break;
    case eCommunications: cout << "eCommunications"; break;
    default: cout << "UNKNOWN";
    }
    cout << endl;
    printDeviceName(pwstrDeviceId);	
    cout << "====================================\n"<< endl;
  }
  defaultDevicedChanged++;
  return S_OK;
}
/* 安装新设备 */
HRESULT STDMETHODCALLTYPE MMNotificationClientImpl::OnDeviceAdded(LPCWSTR pwstrDeviceId)
{
  if (print_switch && pEnumerator!=NULL)
  {
    cout << "--------DeviceAdded--------"<< endl;
    printDeviceName(pwstrDeviceId);
    cout << "===========================\n"<< endl;
  }
  deviceAdded++;
  return S_OK;
}
/* 移除设备 */
HRESULT STDMETHODCALLTYPE MMNotificationClientImpl::OnDeviceRemoved(LPCWSTR pwstrDeviceId)
{
  if (print_switch && pEnumerator!=NULL)
  {
    cout << "--------DeviceRemoved--------"<< endl;
    printDeviceName(pwstrDeviceId);
    cout << "=============================\n"<< endl;
  }
  deviceRemoved++;
  return S_OK;
}
/* 设备状态变化 (启用,禁用,断开等)*/
HRESULT STDMETHODCALLTYPE MMNotificationClientImpl::OnDeviceStateChanged(LPCWSTR pwstrDeviceId,
									 DWORD dwNewState)
{
  if (print_switch && pEnumerator!=NULL)
  {
    cout << "--------DeviceStateChanged--------"<< endl;
    switch (dwNewState)
    {
      case DEVICE_STATE_ACTIVE: cout << "ACTIVE"; break;
      case DEVICE_STATE_DISABLED: cout << "DISABLED"; break;
      case DEVICE_STATE_NOTPRESENT: cout << "NOTPRESENT"; break;
      case DEVICE_STATE_UNPLUGGED: cout << "UNPLUGGED"; break;
      default: cout << "UNKNOWN";
    }
    cout << endl;
    printDeviceName(pwstrDeviceId);
    cout << "==================================\n"<< endl;
  }
  deviceStateChanged++;
  return S_OK;
}
/* 设备属性变化 */
HRESULT STDMETHODCALLTYPE MMNotificationClientImpl::OnPropertyValueChanged(LPCWSTR pwstrDeviceId,
									   const PROPERTYKEY key)
{
  HRESULT hr = S_OK;
  IMMDevice *pDevice = NULL;
  IPropertyStore *pProps = NULL;
  PROPVARIANT varString;
  PropVariantInit(&varString);

  if (print_switch && pEnumerator!=NULL)
  {
    hr = pEnumerator->GetDevice(pwstrDeviceId, &pDevice);

    if (hr == S_OK){hr = pDevice->OpenPropertyStore(STGM_READ, &pProps);}
    if (hr == S_OK){hr = pProps->GetValue(key, &varString);}
    if (hr == S_OK)
    {
      cout << "--------OnPropertyValueChanged--------"<< endl;
      printf("Device Property Key:"
	     "{%8.8x-%4.4x-%4.4x-%2.2x%2.2x-%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x}#%d\n",
	     key.fmtid.Data1, key.fmtid.Data2, key.fmtid.Data3,
	     key.fmtid.Data4[0], key.fmtid.Data4[1],
	     key.fmtid.Data4[2], key.fmtid.Data4[3],
	     key.fmtid.Data4[4], key.fmtid.Data4[5],
	     key.fmtid.Data4[6], key.fmtid.Data4[7],
	     key.pid);
      cout << "Value Type: " << varString.vt << endl;
      cout << "Value (int): " << int(varString.bVal) << endl;
      printDeviceName(pwstrDeviceId, pProps);
      cout << "======================================\n"<< endl;
    }
    
    PropVariantClear(&varString);
    SAFE_RELEASE(pProps)
    SAFE_RELEASE(pDevice)
  }
  propertyValueChanged++;
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MMNotificationClientImpl::QueryInterface(REFIID riid,
								   void **ppvObject)
{
  if (IID_IUnknown == riid)
  {
    AddRef();
    *ppvObject = (IUnknown*)this;
  }
  else if (__uuidof(IMMNotificationClient) == riid)
  {
    AddRef();
    *ppvObject = (IMMNotificationClient*)this;
  }
  else
  {
    *ppvObject = NULL;
    return E_NOINTERFACE;
  }
  return S_OK;
}

ULONG STDMETHODCALLTYPE MMNotificationClientImpl::AddRef(void)
{
  return InterlockedIncrement(&ref);
}
ULONG STDMETHODCALLTYPE MMNotificationClientImpl::Release(void)
{
  ULONG ulRef = InterlockedDecrement(&ref);
  if (0 == ulRef)
  {
    delete this;
  }
  return ulRef;
}

bool MMNotificationClientImpl::isHeadphoneActivated()
{
  bool ret = false;
  if (defaultDevicedChanged == 0 &&
      deviceAdded == 0 &&
      deviceRemoved == 0 &&
      deviceStateChanged == 0 &&
      propertyValueChanged >= 5)//经测试,每次realtek声卡插入耳机都会有至少5个属性变化,拔出耳机无属性变化
  {
    ret = true;
  }
  resetFlags();
  return ret;
}

// 根据设备ID打印设备名称
void MMNotificationClientImpl::printDeviceName(LPCWSTR pwstrId,
					    IPropertyStore* pProps)
{
    PROPERTYKEY key;
    GUID IDevice_FriendlyName = { 0xa45c254e, 0xdf1c, 0x4efd, { 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0 } };
    key.pid = 14;
    key.fmtid = IDevice_FriendlyName;
    
    HRESULT hr = S_OK;
    IMMDevice *pDevice = NULL;
    PROPVARIANT varString;
    PropVariantInit(&varString);

    if( pProps == NULL)
    {
      if (hr == S_OK){hr = pEnumerator->GetDevice(pwstrId, &pDevice);}
      if (hr == S_OK){hr = pDevice->OpenPropertyStore(STGM_READ, &pProps);}
    }

    if (hr == S_OK){hr = pProps->GetValue(key, &varString);}

    if (hr == S_OK)
    {
      printf("Device name: %S\nEndpoint ID string: %S\n",
	     (hr == S_OK) ? varString.pwszVal : L"null device",
	     (pwstrId != NULL) ? pwstrId : L"null ID");
    }
    
    PropVariantClear(&varString);
    
    if ( pProps == NULL)
    {
        SAFE_RELEASE(pProps)
        SAFE_RELEASE(pDevice)
    }
    return;
}

void MMNotificationClientImpl::resetFlags()
{
  defaultDevicedChanged = 0;
  deviceAdded = 0;
  deviceRemoved = 0;
  deviceStateChanged = 0;
  propertyValueChanged = 0;
}
