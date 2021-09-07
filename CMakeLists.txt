project(DetectHeadphoneRunApp)

add_executable(DetectHeadphoneRunApp main.cpp MMNotificationClientImpl.cpp)

target_link_libraries(DetectHeadphoneRunApp PUBLIC ole32 uuid)
