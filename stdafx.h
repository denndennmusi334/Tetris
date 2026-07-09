#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#define COLLIDER_DEBUG false

#define USE_BUTTON_EDIT false

#include <winsock2.h>
#include <ws2tcpip.h>

#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#include <vector>
#include <time.h>
#include <assert.h>
#include <cmath>
#include <type_traits>
#include <limits>
#include <memory>
#include <algorithm>
#include <cstring>
#include <string>
#include <fstream>
#include <functional>
#include <chrono>
#include <variant>
#include <queue>
#include <sstream>
#include <ixwebsocket/IXWebSocket.h>

#include "DxLib.h"

#include <nlohmann/json.hpp>
#pragma warning(push)
#pragma warning(disable:4819)
#include "httplib.h"
#pragma warning(pop)

// 自作ヘッダー
#include "Vector.h"
#include "Config.h"
#include "Mystd.h"