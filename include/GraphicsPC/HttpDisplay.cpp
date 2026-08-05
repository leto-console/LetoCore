#ifdef __TERMUX__

// Принудительное отключение поддержки ZLIB
#ifdef CPPHTTPLIB_ZLIB_SUPPORT
#undef CPPHTTPLIB_ZLIB_SUPPORT
#endif

#include "HTTPDisplay.hpp"
#include "BufferScreen.hpp"

#include <httplib.h>
#include <mutex>

#include <LetoABI/AppEvent.h>
#include <SceneManager/SystemSceneManager.hpp>

static std::mutex g_mutex;
static BufferScreen buffer;

static int g_width, g_height, g_scale;

// ----------------------------------------------------------------------------------------------------

// ===== Хелпер: парсинг строки "B:1:1" → AppEvent =====
static AppEvent ParseInputEvent(const std::string& data) {
    AppEvent evt;
    if (data.size() < 5) return evt; // мин. "X:0:0"
    
    char src_char = data[0];
    // Ищем разделители ':'
    size_t p1 = data.find(':', 1);
    size_t p2 = data.find(':', p1 + 1);
    if (p1 == std::string::npos || p2 == std::string::npos) return evt;
    
    // Парсим ID и тип
    try {
        evt.id = std::stoul(data.substr(p1 + 1, p2 - p1 - 1));
        evt.data = std::stoul(data.substr(p2 + 1));
    } catch (...) {
        return evt;
    }
    
    // Определяем источник
    if (src_char == 'B') {
        evt.source = AE_Button;
    } else if (src_char == 'E') {
        evt.source = AE_Encoder;
    } else {
        evt.source = AE_None;
    }
    
    return evt;
}

static void Handle_Draw(const httplib::Request& req, httplib::Response& res)
{
	std::lock_guard<std::mutex> lock(g_mutex);
	res.set_content(
		reinterpret_cast<const char*>(buffer.image),
		buffer.height * buffer.width * 4,
		"application/octet-stream"
	);
	res.set_header("Cache-Control", "no-store");
}

static void Handle_Input(const httplib::Request& req, httplib::Response& res) 
{
	if (!req.body.empty()) {
		AppEvent evt = ParseInputEvent(req.body);
		if (evt.source != UserInputSource::NONE) {
			std::lock_guard<std::mutex> lock(g_mutex);
			SystemSceneManager::Instance().ProccessUserInput(evt);
		}
	}

	res.set_content("OK", "text/plain");
	res.set_header("Cache-Control", "no-store");
}

static void Handle_DisplayInfo(const httplib::Request& req, httplib::Response& res) 
{
    char response[128];
    //const char* response = R"({"width":128,"height":64,"pixelScale":2})";
    snprintf(response, sizeof(response), R"({"width":%d,"height":%d,"pixelScale":2})", buffer.Width(), buffer.Height());
	res.set_content(response, "application/json");
	res.set_header("Cache-Control", "no-store");
}

static void start_web_server() {
    httplib::Server svr;

    // Draw (получение кадра)
    svr.Get("/draw", Handle_Draw);

    // Input (HTTP POST)
    svr.Post("/input", Handle_Input);

	// Get info
	svr.Get("/display_info", Handle_DisplayInfo);

    //// xx. WebSocket (рекомендуется для real-time)
    //svr.("/ws", [&](auto& ws, const auto&) {
    //    ws.on_message([&](auto&, const std::string& msg) {
    //        std::lock_guard<std::mutex> lock(g_mutex);
    //        for (char c : msg) g_input_queue.push(static_cast<uint8_t>(c));
    //    });
//
    //    // Фоновая отправка кадров клиенту
    //    std::thread sender([&ws]() {
    //        while (ws.is_open()) {
    //            std::this_thread::sleep_for(std::chrono::milliseconds(33)); // ~30 FPS
    //            std::lock_guard<std::mutex> lock(g_mutex);
    //            std::string frame(g_ssd1306_buf, g_ssd1306_buf + 1024);
    //            ws.send(httplib::detail::base64_encode(frame));
    //        }
    //    });
    //    sender.detach();
    //});

    // Статика для отладочной HTML-страницы
    svr.set_mount_point("/", "./web");

    // Запуск (0.0.0.0 для доступа с телефона/браузера)
    svr.listen("0.0.0.0", 12300);
}

// ----------------------------------------------------------------------------------------------------

HttpDisplay::HttpDisplay(int width, int height, int scale) : http_server_thread{ start_web_server }
{
	g_width = width;
	g_height = height;
	g_scale = scale;

    buffer.SetSize(width, height);
	http_server_thread.detach();
}

HttpDisplay::~HttpDisplay()
{
}

int HttpDisplay::Width() const
{
	return g_width;
}

int HttpDisplay::Height() const
{
	return g_height;
}

bool HttpDisplay::IsOpen()
{
    return true;
}

bool HttpDisplay::Init()
{
    return true;
}

void HttpDisplay::Loop()
{
}

bool HttpDisplay::Render(IDrawer* drawer)
{
	std::lock_guard<std::mutex> lock(g_mutex);
	drawer->Draw(buffer);
    return true;
}

#endif /* __TERMUX__ */
