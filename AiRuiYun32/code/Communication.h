#pragma once



//服务端接口
unsigned int __stdcall StartServer(LPVOID lParam);
//通信工作线程
void commu_create_threads();
//关闭程序退出
void app_close();