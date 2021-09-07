#include "mmdeviceapi.h"
#include "windows.h"
#include "unknwn.h"

class MMNotificationClientImpl : public IMMNotificationClient
{
 public:
  MMNotificationClientImpl();
  ~MMNotificationClientImpl();
  /* 默认设备变更 (包括手动设置默认设备,设备插拔导致设备状态变化等情况)*/
  HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(EDataFlow flow, //eRender, eCapture
						   ERole role,//eConsole, eMultimedia, eCommunications
						   LPCWSTR pwstrDeviceId);
  /* 安装新设备 */
  HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR pwstrDeviceId);
  /* 移除设备 */
  HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR pwstrDeviceId);
  /* 设备状态变化 (启用,禁用,断开等)*/
  HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(LPCWSTR pwstrDeviceId,
						 DWORD dwNewState);
  /* 设备属性变化 */
  HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(LPCWSTR pwstrDeviceId,
						   const PROPERTYKEY key);

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
  ULONG STDMETHODCALLTYPE AddRef(void);
  ULONG STDMETHODCALLTYPE Release(void);
  bool isHeadphoneActivated();
  static void setPrintSwitch(bool flag);
 private:
  void printDeviceName(LPCWSTR pwstrId,
		       IPropertyStore* pProps = NULL);
  void resetFlags();
  
  IMMDeviceEnumerator *pEnumerator;
  LONG ref;
  int defaultDevicedChanged;
  int deviceAdded;
  int deviceRemoved;
  int deviceStateChanged;
  int propertyValueChanged;
  static bool print_switch;
};
