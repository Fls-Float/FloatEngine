
#include "FloatApi.h"


#include <thread>
#include <mutex>

#include "gui/include/imgui.h"
#include "gui/include/rlImGui.h"
#include "nfd.hpp"
using namespace nlohmann;
int F_Debug::m_debug = 1;
bool F_Debug::m_showConsole = true;
std::vector<F_Debug::Log_Info> F_Debug::m_logs;
std::vector<std::string> F_Debug::m_commandHistory;
char F_Debug::m_inputBuffer[256] = "";
int F_Debug::m_historyPos = -1;
bool F_Debug::m_scrollToBottom = false;
std::vector<F_Debug::DebugCommand> F_Debug::m_commands;
std::unordered_map<std::string, std::string> F_Debug::m_commandAliases;
std::unordered_set<std::string> F_Debug::m_logFilters;
bool F_Debug::m_showFilteredLogsOnly = false;
std::vector<F_Debug::WatchedVariable> F_Debug::m_watchedVariables;
std::unordered_map<std::string, std::function<std::string(const std::string&)>> F_Debug::m_expressionHandlers;
void F_Debug::RegisterExpressionHandler(const std::string& prefix, std::function<std::string(const std::vector<std::string>&)> handler)
{
	m_expressionHandlers[prefix] = [handler](const std::string& expr) {
		std::vector<std::string> tokens;
		TokenizeCommand(expr, tokens);
		if (tokens.empty()) return std::string("Invalid expression");
		return handler(tokens);
		};
}

std::string F_Debug::EvaluateExpression(const std::string& expression)
{
	// 查找匹配的表达式处理器
	for (const auto& [prefix, handler] : m_expressionHandlers) {
		if (expression.find(prefix) == 0) {
			return handler(expression.substr(prefix.length()));
		}
	}

	// 默认处理：尝试作为变量名
	for (const auto& var : m_watchedVariables) {
		if (var.name == expression) {
			return var.getter();
		}
	}

	return "未知";
	
}



void F_Debug::Init(bool debug)
{
	using namespace WinFuns;
	m_debug = debug;
	if(debug) ShowWindow(GetConsoleWindow(), 1);
	else {
		ShowWindow(GetConsoleWindow(), 0);
	}
}

void F_Debug::Start()
{

	using namespace WinFuns;
	m_debug = 1;
	ShowWindow(GetConsoleWindow(), 1);
}

void F_Debug::Close() {

	using namespace WinFuns;
	m_debug = 0;
   	ShowWindow(GetConsoleWindow(),1);
}

bool F_Debug::IsOpen() {
	return m_debug == 1;
}
void F_Debug::ShowDebugConsole() {
	if (!IsOpen() || !m_showConsole) return;

	const float footer_height = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	ImGui::SetNextWindowSize(ImVec2(600, 450), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("调试控制台", &m_showConsole)) {
		// 工具栏
		if (ImGui::Button("清空")) {
			m_logs.clear();
		}
		ImGui::SameLine();

		// 日志过滤选项
		ImGui::Checkbox("启用过滤", &m_showFilteredLogsOnly);
		ImGui::SameLine();

		// 添加过滤关键词按钮
		static char filterBuf[128] = "";
		ImGui::PushItemWidth(120);
		ImGui::InputText("##FilterInput", filterBuf, IM_ARRAYSIZE(filterBuf));
		ImGui::PopItemWidth();
		ImGui::SameLine();
		if (ImGui::Button("新建过滤内容")) {
			if (filterBuf[0] != '\0') {
				AddLogFilter(filterBuf);
				filterBuf[0] = '\0';
			}
		}

		// 显示当前过滤关键词
		if (!m_logFilters.empty()) {
			ImGui::SameLine();
			ImGui::Text("过滤内容: ");
			for (const auto& filter : m_logFilters) {
				ImGui::SameLine();
				ImGui::Text("%s", filter.c_str());
				ImGui::SameLine();
				if (ImGui::SmallButton("删除")) {
					RemoveLogFilter(filter);
				}
			}
		}

		// 分隔线
		ImGui::Separator();

		// 日志显示区域
		ImGui::BeginChild("日志区", ImVec2(0, -footer_height), false, ImGuiWindowFlags_HorizontalScrollbar);

		// 显示日志（应用过滤）
		for (const auto& logIf : m_logs) {
			const std::string& log = logIf.log;
			// 应用过滤
			bool shouldShow = !m_showFilteredLogsOnly || ShouldShowLog(log);

			if (shouldShow) {
				// 根据日志类型着色
				if (logIf.lv == 3) {
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 50, 50, 255));
				}
				else if (logIf.lv == 2) {
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 200, 50, 255));
				}
				else if (logIf.lv == 1) {
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(50, 255, 50, 255));
				}

				ImGui::TextUnformatted(log.c_str());

				if (logIf.lv ==1 || logIf.lv ==2 || logIf.lv==3) {
					ImGui::PopStyleColor();
				}
			}
		}

		// 自动滚动到底部
		if (m_scrollToBottom) {
			ImGui::SetScrollHereY(1.0f);
			m_scrollToBottom = false;
		}
		ImGui::EndChild();

		// 分隔线
		ImGui::Separator();

		// 变量监控显示
		if (!m_watchedVariables.empty()) {
			ImGui::Text("变量监视:");
			for (const auto& var : m_watchedVariables) {
				if (var.showInConsole) {
					ImGui::Text("%s: %s", var.name.c_str(), var.getter().c_str());
				}
			}
			ImGui::Separator();
		}

		// 命令输入区域
		bool reclaim_focus = false;
		ImGui::PushItemWidth(-1);
		if (ImGui::InputText("##Input", m_inputBuffer, IM_ARRAYSIZE(m_inputBuffer),
			ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory,
			[](ImGuiInputTextCallbackData* data) -> int {
				return F_Debug::TextEditCallback(data);
			}))
		{
			// 执行命令
			if (m_inputBuffer[0] != '\0') {
				ExecuteCommand(m_inputBuffer);

				// 保存到历史记录
				m_commandHistory.push_back(m_inputBuffer);
				if (m_commandHistory.size() > 100) {
					m_commandHistory.erase(m_commandHistory.begin());
				}

				// 重置输入
				strcpy(m_inputBuffer, "");
				reclaim_focus = true;
			}
		}
		ImGui::PopItemWidth();

		// 自动聚焦到输入框
		ImGui::SetItemDefaultFocus();
		if (reclaim_focus) {
			ImGui::SetKeyboardFocusHere(-1);
		}
	}
	ImGui::End();
}


/**
 * @brief 添加日志消息
 * @param message 日志内容
 * @param level 日志级别 (0=none,1=info, 2=warn, 3=error)
 */
void F_Debug::Log(const std::string& message, int level) {
	if (!IsOpen()) return;
	// 添加时间戳
	time_t now = time(0);
	tm* ltm = localtime(&now);
	char timeBuf[20];
	strftime(timeBuf, sizeof(timeBuf), "%H:%M:%S", ltm);

	// 格式化日志
	const char* levelStr = "";
	if (level == 1) levelStr = "[INFO] ";
	else if (level == 2) levelStr = "[WARN] ";
	else if(level==3) levelStr = "[ERROR] ";
	m_logs.push_back({ std::string(timeBuf) + " " + levelStr + message ,level});
	if (m_logs.size() > 500) {
		m_logs.erase(m_logs.begin());
	}

	m_scrollToBottom = true;
}

void F_Debug::ClearLog()
{
	m_logs.clear();
}

/**
 * @brief 注册调试命令
 * @param name 命令名称
 * @param description 命令描述
 * @param action 命令执行函数
 */
void F_Debug::RegisterCommand(const std::string& name, const std::string& description,
	std::function<void(const std::vector<std::string>&)> action)
{
	m_commands.push_back({ name, description, action });
}

std::vector<F_Debug::DebugCommand> F_Debug::GetCommands()
{
	return m_commands;
}

F_Debug::DebugCommand F_Debug::GetCommand(const std::string& name)
{
	F_Debug::DebugCommand cmd;
	auto it = std::find_if(m_commands.begin(), m_commands.end(),
		[&](const DebugCommand& c) { return c.name == name; });
	if (it != m_commands.end()) {
		cmd = *it;
	}
	else {
		Log("未知命令: " + name,2);
		return F_Debug::DebugCommand();
	}
	return cmd;
}

/**
* @brief 执行调试命令
* @param command 完整命令字符串
*/
void F_Debug::ExecuteCommand(const std::string& command) {
	// 添加日志
	Log(" [指令]> " + command);

	// 解析命令和参数
	std::vector<std::string> tokens;
	TokenizeCommand(command, tokens);

	if (tokens.empty()) return;

	// 查找命令
	std::string cmdName = tokens[0];
	tokens.erase(tokens.begin()); // 移除命令名，保留参数

	auto it = std::find_if(m_commands.begin(), m_commands.end(),
		[&](const DebugCommand& cmd) { return cmd.name == cmdName; });

	if (it != m_commands.end()) {
		try {
			it->action(tokens);
		}
		catch (const std::exception& e) {
			Log("执行命令错误: " + std::string(e.what()), 3);
		}
	}
	else {
		Log("未知命令: " + cmdName, 2);
	}
}

void F_Debug::InitCommand()
{
	F_Debug::RegisterCommand("clear", "清空所有日志\n", [](const auto& args) {
		F_Debug::ClearLog();
		});
	F_Debug::RegisterCommand("help", "帮助\n", [](const auto& args) {
		F_Debug::Log("可用的调试命令:");
		std::string tmp = "\n";
		for (auto cmd : F_Debug::GetCommands()) {
			tmp += (TextFormat("%s:\n    %s", cmd.name.c_str(), cmd.description.c_str()));
		}
		F_Debug::Log(tmp);
		});
	F_Debug::RegisterCommand("logsave",
		"保存日志到文件\n",
		[](const auto& args) {

			std::string filename = "logs.txt";
			bool filtered = false;
			if (args[0] == "/?") {
				F_Debug::Log("保存日志到文件\n    logsave [filename] [filtered]\n    filename - 文件名(可省略)\n    filtered - 指定保存过滤的日志(可省略)\n    例:logsave logs.txt filtered");
				return;
			}
			if (args.size() > 0) filename = args[0];
			if (args.size() > 1 && args[1] == "filtered") filtered = true;

			F_Debug::SaveLogsToFile(filename, filtered);
		});
	F_Debug::RegisterCommand("filter", "管理日志过滤\n", [](const auto& args) {
		if (args.empty() || args[0] == "/?") {
			F_Debug::Log("管理日志过滤\n    用法: filter [add|remove|list] [keyword]\n    例:filter add [ERROR]\n    filter remove [WARN]");
			return;
		}

		if (args[0] == "add" && args.size() > 1) {
			F_Debug::AddLogFilter(args[1]);
		}
		else if (args[0] == "remove" && args.size() > 1) {
			F_Debug::RemoveLogFilter(args[1]);
		}
		else if (args[0] == "list") {
			if (F_Debug::m_logFilters.empty()) {
				F_Debug::Log("没有有效的过滤内容");
			}
			else {
				F_Debug::Log("有效的过滤内容:");
				for (const auto& filter : F_Debug::m_logFilters) {
					F_Debug::Log("  " + filter);
				}
			}
		}
		});
	RegisterCommand("watch", "管理监视的变量\n", [](const std::vector<std::string>& args) {
		if (args.empty() || args[0]=="/?") {
			std::string tmp = "";
			tmp += ("用法:\n");
			tmp += ("  watch list                     - 列出所有被监视的变量\n");
			tmp += ("  watch add <name> <expression>  - 添加一个新的被监视的变量\n");
			tmp += ("  watch remove <name>            - 移除一个被监视的变量\n");
			tmp += ("  watch show <name>              - 显示一个被监视的变量\n");
			tmp += ("  watch hide <name>              - 隐藏一个被监视的变量\n");
			tmp += ("  watch eval <expression>        - 计算表达式\n");
			Log(tmp);
			return;
		}

		const std::string& action = args[0];

		if (action == "list") {
			if (m_watchedVariables.empty()) {
				Log("没有被监视的变量");
			}
			else {
				Log("被监视的变量:");
				for (const auto& var : m_watchedVariables) {
					Log("  " + var.name + " = " + var.lastValue +
						" (" + var.expression + ")" +
						" [显示状态: " + (var.showInConsole ? "显示" : "不显示") + "]");
				}
			}
		}
		else if (action == "add" && args.size() >= 3) {
			std::string name = args[1];
			std::string expr = args[2];
			// 合并剩余参数作为表达式
			for (int i = 3; i < args.size(); i++) {
				expr += " " + args[i];
			}
			WatchVariable(name, expr);
		}
		else if (action == "remove" && args.size() >= 2) {
			UnwatchVariable(args[1]);
		}
		else if (action == "show" && args.size() >= 2) {
			SetVariableVisibility(args[1], true);
		}
		else if (action == "hide" && args.size() >= 2) {
			SetVariableVisibility(args[1], false);
		}
		else if (action == "eval" && args.size() >= 2) {
			std::string expr = args[1];
			for (int i = 2; i < args.size(); i++) {
				expr += " " + args[i];
			}
			Log("结果为: " + EvaluateExpression(expr));
		}
		else {
			Log("无效的命令", 1);
		}
		});

	RegisterExpressionHandler("Math.", [](const std::vector<std::string>& tokens) {
		// 示例：math.sin(1.57), math.add(5,3)
		if (tokens.size() < 2) return std::string("Invalid math expression");

		try {
			if (tokens[0] == "sin") {
				float value = std::stof(tokens[1]);
				return std::to_string(sinf(value));
			}
			else if (tokens[0] == "cos") {
				float value = std::stof(tokens[1]);
				return std::to_string(cosf(value));
			}
			else if (tokens[0] == "tan") {
				float value = std::stof(tokens[1]);
				return std::to_string(tanf(value));
			}
			else if (tokens[0] == "sqrt") {
				float value = std::stof(tokens[1]);
				if (value < 0) return std::string("负数不能开方");
				return std::to_string(sqrtf(value));
			}
			else if (tokens[0] == "abs") {
				float value = std::stof(tokens[1]);
				return std::to_string(fabsf(value));
			}
			else if (tokens[0] == "pow" && tokens.size() > 2) {
				float base = std::stof(tokens[1]);
				float exponent = std::stof(tokens[2]);
				return std::to_string(powf(base, exponent));
			}
			else if (tokens[0] == "log" && tokens.size() > 1) {
				float value = std::stof(tokens[1]);
				if (value <= 0) return std::string("对数的输入必须大于0");
				return std::to_string(logf(value));
			}
			else if (tokens[0] == "exp" && tokens.size() > 1) {
				float value = std::stof(tokens[1]);
				return std::to_string(expf(value));
			}
			else if (tokens[0] == "ceil" && tokens.size() > 1) {
				float value = std::stof(tokens[1]);
				return std::to_string(ceilf(value));
			}
			else if (tokens[0] == "floor" && tokens.size() > 1) {
				float value = std::stof(tokens[1]);
				return std::to_string(floorf(value));
			}
			else if (tokens[0] == "round" && tokens.size() > 1) {
				float value = std::stof(tokens[1]);
				return std::to_string(roundf(value));
			}
			else if (tokens[0] == "max" && tokens.size() > 2) {
				float a = std::stof(tokens[1]);
				float b = std::stof(tokens[2]);
				return std::to_string(std::max(a, b));
			}
			else if (tokens[0] == "min" && tokens.size() > 2) {
				float a = std::stof(tokens[1]);
				float b = std::stof(tokens[2]);
				return std::to_string(std::min(a, b));
			}

			else if (tokens[0] == "add" && tokens.size() > 2) {
				float a = std::stof(tokens[1]);
				float b = std::stof(tokens[2]);
				return std::to_string(a + b);
			}
			else if (tokens[0] == "sub" && tokens.size() > 2) {
				float a = std::stof(tokens[1]);
				float b = std::stof(tokens[2]);
				return std::to_string(a - b);
			}
		}
		catch (...) {
			return std::string("数学表达式错误");
		}
		return "未知数学函数: " + tokens[0];
		});
	RegisterExpressionHandler("System.", [](const std::vector<std::string>& tokens) {
		// 示例：system.fps, system.memory
		if (tokens.empty()) return std::string("Invalid system expression");

		if (tokens[0] == "fps") {
			return std::to_string(GetFPS());
		}
		else if (tokens[0] == "time") {
			return std::to_string(GetTime());
		}
		return "未知系统属性: " + tokens[0];
		});
}

/**
* @brief 命令分词
* @param input 输入命令
* @param tokens 输出分词结果
*/
void F_Debug::TokenizeCommand(const std::string& input, std::vector<std::string>& tokens) {
	tokens.clear();
	char* str = const_cast<char*>(input.c_str());
	char* token = strtok(str, " ");

	while (token != nullptr) {
		tokens.push_back(token);
		token = strtok(nullptr, " ");
	}
}

void F_Debug::AddLogFilter(const std::string& keyword)
{
	m_logFilters.insert(keyword);
//	Log("添加了日志过滤内容: " + keyword,1);
}

void F_Debug::RemoveLogFilter(const std::string& keyword)
{
	std::string tmp = keyword;
	if (m_logFilters.erase(keyword) > 0) {
	//	Log("移除了日志过滤内容: " + tmp,1);
	}
}

bool F_Debug::ShouldShowLog(const std::string& log)
{
	if (m_logFilters.empty()) return true;

	for (const auto& filter : m_logFilters) {
		if (log.find(filter) != std::string::npos) {
			return true;
		}
	}
	return false;
}
void F_Debug::SaveLogsToFile(const std::string& filename, bool filtered)
{
	std::ofstream file(filename);
	if (!file.is_open()) {
		Log("打开日志文件失败: " + filename, 3);
		return;
	}

	int count = 0;
	for (const auto& logIf : m_logs) {
		auto log = logIf.log;
		if (!filtered || ShouldShowLog(log)) {
			file << log << "\n";
			count++;
		}
	}

	file.close();
	Log("保存 " + std::to_string(count) + " 条日志到 " + filename,1);
}
void F_Debug::RegisterAlias(const std::string& alias, const std::string& command)
{
	m_commandAliases[alias] = command;
	Log("注册命令别名: " + alias + " -> " + command);
}
void F_Debug::WatchVariable(const std::string& name, std::function<std::string()> getter, bool showInConsole)
{
	auto it = std::find_if(m_watchedVariables.begin(), m_watchedVariables.end(),
		[&](const WatchedVariable& v) { return v.name == name; });

	if (it != m_watchedVariables.end()) {
		Log("变量 '" + name + "' 以及被监视了", 1);
		return;
	}

	WatchedVariable t;
	t.expression = "";
	t.getter = getter;
	t.lastValue = getter();
	t.name = name;
	t.showInConsole = showInConsole;
	m_watchedVariables.push_back(t);
	Log("开始监听变量: " + name);
}
void F_Debug::WatchVariable(const std::string& name, const std::string& expression, bool showInConsole)

{
	// 检查是否已存在同名变量
	auto it = std::find_if(m_watchedVariables.begin(), m_watchedVariables.end(),
		[&](const WatchedVariable& v) { return v.name == name; });

	if (it != m_watchedVariables.end()) {
		Log("变量 '" + name + "' 以及被监视了", 1);
		return;
	}

	// 创建获取函数
	auto getter = [expression]() -> std::string {
		return EvaluateExpression(expression);
		};

	// 获取初始值
	std::string initialValue;
	try {
		initialValue = getter();
	}
	catch (...) {
		initialValue = "Error";
	}

	// 添加监视变量
	m_watchedVariables.push_back({
		name,
		expression,
		getter,
		showInConsole,
		initialValue
		});

	Log("开始监听变量: " + name);
}
void F_Debug::UnwatchVariable(const std::string& name)
{
	auto it = std::remove_if(m_watchedVariables.begin(), m_watchedVariables.end(),
		[&](const WatchedVariable& v) { return v.name == name; });

	if (it != m_watchedVariables.end()) {
		m_watchedVariables.erase(it, m_watchedVariables.end());
		Log("停止监视变量: " + name);
	}
	else {
		Log("未找到变量: " + name, 1);
	}
}
void F_Debug::SetVariableVisibility(const std::string& name, bool show)
{
	for (auto& var : m_watchedVariables) {
		if (var.name == name) {
			var.showInConsole = show;
			Log("'" + name + "'的可视状态设置为" + (show ? "显示" : "隐藏"));
			return;
		}
	}
	Log("没有找到变量: " + name, 1);
}
void F_Debug::UpdateWatchedVariables()
{
	static int frameCount = 0;
	frameCount++;

	// 每5帧更新一次，避免性能开销
	if (frameCount % 5 == 0) {
		for (auto& var : m_watchedVariables) {
			try {
				std::string currentValue = var.getter();

				// 检测值变化
				if (var.lastValue != currentValue) {
					// 记录变化日志
					Log(var.name + " 变化: " + var.lastValue + " -> " + currentValue);
					var.lastValue = currentValue;
				}
			}
			catch (const std::exception& e) {
				Log("表达式计算错误 " + var.name + ": " + e.what(), 2);
			}
		}
	}
}
/**
* @brief 输入框回调函数 (处理自动补全和历史记录)
* @param data ImGui回调数据
*/
int F_Debug::TextEditCallback(ImGuiInputTextCallbackData* data) {
	switch (data->EventFlag) {
	case ImGuiInputTextFlags_CallbackHistory: {
		// 上/下方向键处理历史记录
		const int prev_history_pos = m_historyPos;
		if (data->EventKey == ImGuiKey_UpArrow) {
			if (m_historyPos == -1) {
				m_historyPos = (int)m_commandHistory.size() - 1;
			}
			else if (m_historyPos > 0) {
				m_historyPos--;
			}
		}
		else if (data->EventKey == ImGuiKey_DownArrow) {
			if (m_historyPos != -1) {
				if (++m_historyPos >= (int)m_commandHistory.size()) {
					m_historyPos = -1;
				}
			}
		}

		// 应用历史记录
		if (prev_history_pos != m_historyPos) {
			const char* history_str = (m_historyPos >= 0) ? m_commandHistory[m_historyPos].c_str() : "";
			data->DeleteChars(0, data->BufTextLen);
			data->InsertChars(0, history_str);
		}
		break;
	}
	case ImGuiInputTextFlags_CallbackCompletion: {
		// Tab键自动补全
		const char* word_end = data->Buf + data->CursorPos;
		const char* word_start = word_end;
		while (word_start > data->Buf) {
			const char c = word_start[-1];
			if (c == ' ' || c == '\t') break;
			word_start--;
		}

		// 收集匹配的命令
		std::vector<std::string> candidates;
		const std::string partial(word_start, word_end);

		for (const auto& cmd : m_commands) {
			if (cmd.name.find(partial) == 0) {
				candidates.push_back(cmd.name);
			}
		}

		if (!candidates.empty()) {
			// 自动补全
			if (candidates.size() == 1) {
				data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
				data->InsertChars(data->CursorPos, candidates[0].c_str());
				data->InsertChars(data->CursorPos, " ");
			}
			// 显示多个匹配项
			else {
				Log("可能结果:");
				for (const auto& candidate : candidates) {
					Log("  " + candidate);
				}
			}
		}
		break;
	}
	}
	return 0;
}
std::mutex log_mutex; // 全局互斥锁

void DEBUG_LOG(int lv, const char* text, bool english, bool auto_enter , bool with_imgui_console ) {
	if (!F_Debug::IsOpen()) return;

	std::lock_guard<std::mutex> lock(log_mutex); // 自动加锁

	char buffer[1024];
	size_t pos = 0;

	// 构建前缀
	auto append = [&](const char* str) {
		size_t len = snprintf(buffer + pos, sizeof(buffer) - pos, "%s", str);
		pos += std::min(len, sizeof(buffer) - pos - 1);
		};

	switch (lv) {
	case LOG_ALL:
		if (english)
			append("LOG:");
		else append("日志:");
		break;
	case LOG_DEBUG:
		if (english)
			append("DEBUG:");
		else append("调试：");
		break;
	case LOG_ERROR:
		if (english)
			append("ERROR:");
		else append("错误:");
		break;
	case LOG_FATAL:
		if (english)
			append("FATAL:");
		else append("致命错误:");
		break;
	case LOG_INFO:
		if (english)
			append("INFO:");
		else append("信息:");
		break;
	case LOG_NONE:
		if (english)
			append("NONE:");
		else append("无:");
		break;
	case LOG_WARNING:
		if (english)
			append("WARNING:");
		else append("警告:");
		break;
	default:
		if (english)
			append("NONE:");
		else append("无:");
		break;
	};
	append(text);
	if (auto_enter) append("\n");

	fwrite(buffer, 1, pos, stdout); // 单次原子操作
	if (with_imgui_console) {
		append("\0");
		if (lv == LOG_INFO) lv = 1;
		else if (lv == LOG_WARNING) lv = 2;
		else if (lv == LOG_ERROR) lv = 3;
		else lv = 0;
		F_Debug::Log(buffer, lv); // 添加到调试控制台
	}
}
int letter_to_kv(char letter)
{
	return F_Input::to_value(letter);
}
int number_to_kv(int number) {
	return F_Input::number_to_value(number);
}
Color Make_Color_RGB(int r, int g, int b, float a) {
	return ColorAlpha(Color{ (unsigned char)r,(unsigned char)g,(unsigned char)b }, a);
}
F_Color Make_FColor_RGB(int r, int g, int b, float a) {
	F_Color t;
	t.R = r;
	t.G = g;
	t.B = b;
	t.A = a;
	return t;
}
void F_Render::Draw_Set_Line_Thick(float thick)
{
	_draw_line_thick = thick;
}
void F_Render::Draw_Sprite_Ex(Texture* img, int ind, float x, float y, float x_scale, float y_scale, float x_origin, float y_origin, float angle, float alpha, Color col) {
	if (ind == 0) DrawTexturePro(*img, { 0,0,(*img).width * 1.0f,(*img).height * 1.0f }, { x,y,(*img).width * x_scale,(*img).height * y_scale }, { x_origin * (*img).width * x_scale,y_origin * (*img).height * y_scale }, angle, ColorAlpha(col, alpha));
	else DrawTexturePro(img[ind], { 0,0,img[ind].width * 1.0f ,img[ind].height * 1.0f }, { x,y,img[ind].width * x_scale,img[ind].height * y_scale }, { x_origin * img[ind].width * x_scale,y_origin * img[ind].height * y_scale }, angle, ColorAlpha(col, alpha));
}

void F_Render::Draw_Sprite(Sprite spr, float x, float y, float x_scale, float y_scale, float x_origin, float y_origin, float angle, float alpha, Color col)
{
	Texture img = spr.GetCurrentFrame();
	DrawTexturePro(img, { 0,0,(img).width * 1.0f,(img).height * 1.0f }, { x,y,(img).width * x_scale,(img).height * y_scale }, { x_origin * (img).width * x_scale,y_origin * (img).height * y_scale }, angle, ColorAlpha(col, alpha));
}

void Audio_Play(F_Audio& audio, bool loop = false) {
	if (audio.GetMode() == AudioMode::NONE) {
		DEBUG_LOG(LOG_WARNING, "Audio_Play: 未初始化的audio!", false);
		return;
	}
	audio.Play(loop);
}

void Audio_Stop(F_Audio& audio) {
	if (audio.GetMode() == AudioMode::NONE) {
		DEBUG_LOG(LOG_WARNING, "Audio_Stop: 未初始化的audio!", false);
		return;
	}

	audio.Stop();
}

void Audio_Music_Update(F_Audio& audio, Vector2 listenerPos = { 0.0f, 0.0f }) {
	if (audio.GetMode() != AudioMode::MUSIC) {
		DEBUG_LOG(LOG_ERROR, "Audio_Music_Update: 只能对 Music 进行更新!", false);
		return;
	}

	audio.UpdateSpatialPosition(listenerPos);
}

Font Load_Font(const char* filePath, int size,const char*m_CodePoint)
{
	int dataSize;
	int count;
	unsigned char* fileData = LoadFileData(filePath, &dataSize);
	int* codepoints = LoadCodepoints(m_CodePoint, &count);
	Font f=LoadFontFromMemory(GetFileExtension(filePath), fileData, dataSize, size, codepoints, count);
	return f;
}

Font Load_Font_File_Codepoints(const char* filePath, int size, const char* codePoints_File)
{
	const char* codepoint = LoadFileText(codePoints_File);
	return Load_Font(filePath, size, codepoint);
}
// 在类外赋值
const int F_Input::key_null = 0;
const int F_Input::key_apostrophe = 39;
const int F_Input::key_comma = 44;
const int F_Input::key_minus = 45;
const int F_Input::key_period = 46;
const int F_Input::key_slash = 47;
const int F_Input::key_zero = 48;
const int F_Input::key_one = 49;
const int F_Input::key_two = 50;
const int F_Input::key_three = 51;
const int F_Input::key_four = 52;
const int F_Input::key_five = 53;
const int F_Input::key_six = 54;
const int F_Input::key_seven = 55;
const int F_Input::key_eight = 56;
const int F_Input::key_nine = 57;
const int F_Input::key_semicolon = 59;
const int F_Input::key_equal = 61;
const int F_Input::key_a = 65;
const int F_Input::key_b = 66;
const int F_Input::key_c = 67;
const int F_Input::key_d = 68;
const int F_Input::key_e = 69;
const int F_Input::key_f = 70;
const int F_Input::key_g = 71;
const int F_Input::key_h = 72;
const int F_Input::key_i = 73;
const int F_Input::key_j = 74;
const int F_Input::key_k = 75;
const int F_Input::key_l = 76;
const int F_Input::key_m = 77;
const int F_Input::key_n = 78;
const int F_Input::key_o = 79;
const int F_Input::key_p = 80;
const int F_Input::key_q = 81;
const int F_Input::key_r = 82;
const int F_Input::key_s = 83;
const int F_Input::key_t = 84;
const int F_Input::key_u = 85;
const int F_Input::key_v = 86;
const int F_Input::key_w = 87;
const int F_Input::key_x = 88;
const int F_Input::key_y = 89;
const int F_Input::key_z = 90;
const int F_Input::key_left_bracket = 91;
const int F_Input::key_backslash = 92;
const int F_Input::key_right_bracket = 93;
const int F_Input::key_grave = 96;
const int F_Input::key_space = 32;
const int F_Input::key_escape = 256;
const int F_Input::key_enter = 257;
const int F_Input::key_tab = 258;
const int F_Input::key_backspace = 259;
const int F_Input::key_insert = 260;
const int F_Input::key_delete = 261;
const int F_Input::key_right = 262;
const int F_Input::key_left = 263;
const int F_Input::key_down = 264;
const int F_Input::key_up = 265;
const int F_Input::key_page_up = 266;
const int F_Input::key_page_down = 267;
const int F_Input::key_home = 268;
const int F_Input::key_end = 269;
const int F_Input::key_caps_lock = 280;
const int F_Input::key_scroll_lock = 281;
const int F_Input::key_num_lock = 282;
const int F_Input::key_print_screen = 283;
const int F_Input::key_pause = 284;
const int F_Input::key_f1 = 290;
const int F_Input::key_f2 = 291;
const int F_Input::key_f3 = 292;
const int F_Input::key_f4 = 293;
const int F_Input::key_f5 = 294;
const int F_Input::key_f6 = 295;
const int F_Input::key_f7 = 296;
const int F_Input::key_f8 = 297;
const int F_Input::key_f9 = 298;
const int F_Input::key_f10 = 299;
const int F_Input::key_f11 = 300;
const int F_Input::key_f12 = 301;
const int F_Input::key_left_shift = 340;
const int F_Input::key_left_control = 341;
const int F_Input::key_left_alt = 342;
const int F_Input::key_left_super = 343;
const int F_Input::key_right_shift = 344;
const int F_Input::key_right_control = 345;
const int F_Input::key_right_alt = 346;
const int F_Input::key_right_super = 347;
const int F_Input::key_kb_menu = 348;
const int F_Input::key_kp_0 = 320;
const int F_Input::key_kp_1 = 321;
const int F_Input::key_kp_2 = 322;
const int F_Input::key_kp_3 = 323;
const int F_Input::key_kp_4 = 324;
const int F_Input::key_kp_5 = 325;
const int F_Input::key_kp_6 = 326;
const int F_Input::key_kp_7 = 327;
const int F_Input::key_kp_8 = 328;
const int F_Input::key_kp_9 = 329;
const int F_Input::key_kp_decimal = 330;
const int F_Input::key_kp_divide = 331;
const int F_Input::key_kp_multiply = 332;
const int F_Input::key_kp_subtract = 333;
const int F_Input::key_kp_add = 334;
const int F_Input::key_kp_enter = 335;
const int F_Input::key_kp_equal = 336;
const int F_Input::key_back = 4;
const int F_Input::key_menu = 82;
const int F_Input::key_volume_up = 24;
const int F_Input::key_volume_down = 25;
// 鼠标按钮赋值
const int F_Input::mouse_button_left = 0;
const int F_Input::mouse_button_right = 1;
const int F_Input::mouse_button_middle = 2;
const int F_Input::mouse_button_side = 3;
const int F_Input::mouse_button_extra = 4;
const int F_Input::mouse_button_forward = 5;
const int F_Input::mouse_button_back = 6;
bool F_Input::keyboard_down(int key)
{
	return IsKeyDown(key);
}
bool F_Input::mb_down(int mb)
{
	return IsMouseButtonDown(mb);
}
bool F_Input::mb_pressed(int mb)
{
	return IsMouseButtonPressed(mb);
}
bool F_Input::mb_released(int mb)
{
	return IsMouseButtonReleased(mb);
}
bool F_Input::mb_up(int mb)
{
	return IsMouseButtonUp(mb);
}
bool F_Input::keyboard_pressed(int key) {
	return IsKeyPressed(key);
}
bool F_Input::keyboard_released(int key) {
	return IsKeyReleased(key);
}
bool F_Input::keyboard_pressed_repeat(int key){
	return IsKeyPressedRepeat(key);
}
int F_Input::to_value(char letter)
{
	char c = toupper(letter);
	if (c >= 'A' && c <= 'Z') {
		return c - 'A' + 65;
	}
	return key_null;
}

int F_Input::number_to_value(int number)
{
	if (number >= 0 && number <= 9) {
		return number + 48;
	}
	return key_null;
}

Color F_Color::LightGray = LIGHTGRAY;
Color F_Color::Gray = GRAY;
Color F_Color::DarkGray = DARKGRAY;
Color F_Color::DrakGray = YELLOW;
Color F_Color::Gold = GOLD;
Color F_Color::Orange = ORANGE;
Color F_Color::Pink = PINK;
Color F_Color::Red = RED;
Color F_Color::Maroon = MAROON;
Color F_Color::Green = GREEN;
Color F_Color::Lime = LIME;
Color F_Color::DarkGreen = DARKGREEN;
Color F_Color::SkyBlue = SKYBLUE;
Color F_Color::Blue = BLUE;
Color F_Color::DarkBlue = DARKBLUE;
Color F_Color::Purple = PURPLE;
Color F_Color::Violet = VIOLET;
Color F_Color::Darkpurple = DARKPURPLE;
Color F_Color::Beige = BEIGE;
Color F_Color::Borwn = BROWN;
Color F_Color::DarkBrown = DARKBROWN;
Color F_Color::White = WHITE;
Color F_Color::Black = BLACK;
Color F_Color::Blank = BLANK;
Color F_Color::Magenta = MAGENTA;
Color F_Color::RayWhite = RAYWHITE;
Color F_Color::Yellow = YELLOW;

float F_Render::_draw_line_thick = 1.0f;

void F_Render::Draw_Text_Ex(Font font, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing,float size, Color col, float alpha) {
	DrawTextPro(font, text, { x,y }, { o_x*MeasureTextEx(font,text,size,spacing).x,o_y*MeasureTextEx(font,text,size,spacing).y }, rot, size, spacing, ColorAlpha(col, alpha));
}
void F_Render::Draw_Shape( F_Shape shape,Color col)
{
	if (shape.Get_Points_Number()<=0) {
		DEBUG_LOG(LOG_WARNING, "Draw_Shape:没有点存在", 0);
	}
	for (int i=0; i < shape.Get_Points_Number()-1; i++) {
		DrawLineV(shape.Get_Points()[i], shape.Get_Points()[i+1],  col);
	}
	DrawLineV(shape.Get_Points()[shape.Get_Points_Number()-1], shape.Get_Points()[0],  col);

}

void F_Render::Draw_Rectangle(F_Rectangle rect, Color col)
{
	DrawRectanglePro({rect.x,rect.y,rect.width,rect.height}, {rect.rot_origin.x * rect.width,rect.rot_origin.y * rect.height}, rect.angle, col);
}

void F_Render::Draw_Circle(F_Circle circle, Color col)
{
	DrawCircle(circle.x, circle.y, circle.radius, col);
}

void F_Render::Draw_Ellipse(F_Ellipse ellipse, Color col)
{
	DrawEllipse(ellipse.x, ellipse.y, ellipse.radiusX, ellipse.radiusY, col);
}

void F_Render::Draw_Shape_Fill(F_Shape shape, Color col)
{
	Draw_Polygon(shape.Get_Points(), col);
}

void F_Render::Draw_Polygon(F_Polygon polygon, Color col)
{
	for (int i = 0; i < polygon.Get_Edge_Number(); i++) {
		Draw_Line(polygon.Get_Edge(i), col);
	}
}

void F_Render::Draw_Polygon(std::vector<FVec2> points, Color color) {
	int pointsCount = static_cast<int>(points.size());
	// 确保点的数量有效
	if (pointsCount < 3) return; // 至少需要三个点

	// 计算并绘制多边形的边
	for (int i = 0; i < pointsCount; i++) {
		int nextIndex = (i + 1) % pointsCount; // 下一个点的索引
		DrawLineV(points[i], points[nextIndex], color); // 绘制边
	}

	// 填充多边形
	// 使用 raylib 提供的 DrawTriangleFan 来填充多边形
	for (int i = 1; i < pointsCount - 1; i++) {
		DrawTriangle(points[0], points[i], points[i + 1], color);
	}
}

void F_Render::Draw_Triangle(F_Triangle triangle, Color col)
{
	DrawTriangle(triangle.a, triangle.b, triangle.c, col);
}

void F_Render::Draw_Line(F_Line line, Color col)
{
	DrawLine(line.p1.x, line.p1.y, line.p2.x, line.p2.y, col);
}

void F_Render::Draw_Line(float x1, float y1, float x2, float y2, Color col)
{
	DrawLine(x1, y1, x2, y1, col);
}

void F_Render::Draw_Rectangle(float x, float y, float w, float h, Color col, bool line_mode)
{
	if(!line_mode)
		DrawRectangle(x, y, w, h, col);
	else {
		DrawRectangleLinesEx({ x,y,w,h }, _draw_line_thick, col);
	}
}

void F_Render::Draw_Rectangle(float x, float y, float w, float h, float rot, FVec2 origin, Color col, bool line_mode)
{
	if(!line_mode)
		DrawRectanglePro({ x,y,w,h }, {origin.x,origin.y}, rot, col);
	else {
		// 计算矩形的半宽和半高
		float hw = w / 2.0f;
		float hh = h / 2.0f;

		// 基于原点计算矩形的四个顶点（未经旋转）
		FVec2 vertices[4] = {
			{ -hw - origin.x, -hh - origin.y },  // 左下角
			{  hw - origin.x, -hh - origin.y },  // 右下角
			{  hw - origin.x,  hh - origin.y },  // 右上角
			{ -hw - origin.x,  hh - origin.y }   // 左上角
		};

		// 旋转矩阵的正余弦值
		float cosRot = cosf(rot);
		float sinRot = sinf(rot);

		// 用来保存旋转后的顶点
		Vector2 transformedVertices[4];

		// 计算旋转和平移后的顶点位置
		for (int i = 0; i < 4; ++i) {
			float rotatedX = vertices[i].x * cosRot - vertices[i].y * sinRot;
			float rotatedY = vertices[i].x * sinRot + vertices[i].y * cosRot;
			transformedVertices[i] = { rotatedX + x, rotatedY + y };
		}
		for (int i = 0; i < 4; i++) {
			for (int i = 0; i < 4; ++i) {
				int next = (i + 1) % 4; // 下一个顶点，形成一个循环
				DrawLineV(transformedVertices[i], transformedVertices[next], col);
			}
		}
	}
}

void F_Render::Draw_Circle(float x, float y, float radius, Color col, bool line_mode)
{
	if(!line_mode)
		DrawCircle(x, y, radius, col);
	else {
		DrawCircleLines(x, y, radius, col);
	}
}

void F_Render::Draw_Circle(float x, float y, float radius, float startAngle, float angle, Color col, bool line_mode)
{
	if(!line_mode)
		DrawCircleSector({ x,y }, radius, startAngle, startAngle + angle, angle, col);
	else {
		DrawCircleSectorLines({ x,y }, radius, startAngle, startAngle + angle, angle, col);
	}
}

void F_Render::Draw_Ellipse(float x, float y, float a, float b, Color col, bool line_mode )
{
	if (!line_mode)
		DrawEllipse(x, y, a, b, col);
	else {
		DrawEllipseLines(x, y, a, b, col);
	}
}

void F_Render::Draw_Triangle(float x1, float y1, float x2, float y2, float x3, float y3, Color col, bool line_mode)
{
	if (!line_mode)
		DrawTriangle({ x1,y1 }, { x2,y2 }, { x3,y3 }, col);
	else {
		DrawTriangleLines({ x1,y1 }, { x2,y2 }, { x3,y3 }, col);
	}
}
void F_Render::Draw_Triangle(float x1, float y1, float x2, float y2, float x3, float y3, float rot, Color col, bool line_mode) {
	using namespace floatapi_math;
	if (!line_mode) {
		float centerX = (x1 + x2 + x3) / 3.0f;
		float centerY = (y1 + y2 + y3) / 3.0f;
		FVec2 p1 = Rotate_Point({ x1, y1 }, { centerX, centerY }, rot);
		FVec2 p2 = Rotate_Point({ x2, y2 }, { centerX, centerY }, rot);
		FVec2 p3 = Rotate_Point({ x3, y3 }, { centerX, centerY }, rot);
		DrawTriangle(p1, p2, p3, col);
	}
	else {
		float centerX = (x1 + x2 + x3) / 3.0f;
		float centerY = (y1 + y2 + y3) / 3.0f;
		FVec2 p1 = Rotate_Point({ x1, y1 }, { centerX, centerY }, rot);
		FVec2 p2 = Rotate_Point({ x2, y2 }, { centerX, centerY }, rot);
		FVec2 p3 = Rotate_Point({ x3, y3 }, { centerX, centerY }, rot);
		DrawTriangleLines(p1, p2, p3, col);
	}
}
void F_Render::Draw_Text_Outline(struct Font fnt, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float font_size, float thick, Color col, Color outline_color, float alpha ,bool fill_all)
{
	Font _draw_font = fnt;
	if (!fill_all) {
		Draw_Text_Ex(fnt, text, x - thick, y, o_x, o_y, rot, spacing, font_size, outline_color, alpha);
		Draw_Text_Ex(fnt, text, x + thick, y, o_x, o_y, rot, spacing, font_size, outline_color, alpha);
		Draw_Text_Ex(fnt, text, x, y - thick, o_x, o_y, rot, spacing, font_size, outline_color, alpha);
		Draw_Text_Ex(fnt, text, x, y + thick, o_x, o_y, rot, spacing, font_size, outline_color, alpha);
	}
	else {
		for (int i = 1; i <= (int)(thick); i++) {
			Draw_Text_Ex(fnt, text, x - i, y, o_x, o_y, rot, spacing, font_size, outline_color, alpha);
			Draw_Text_Ex(fnt, text, x + i, y, o_x, o_y, rot, spacing, font_size, outline_color, alpha);
			Draw_Text_Ex(fnt, text, x, y - i, o_x, o_y, rot, spacing, font_size, outline_color, alpha);
			Draw_Text_Ex(fnt, text, x, y + i, o_x, o_y, rot, spacing, font_size, outline_color, alpha);
		}
	}
	Draw_Text_Ex(fnt, text, x, y, o_x, o_y, rot, spacing, font_size, col, alpha);
}
void F_Render::Draw_Text_Shadow( Font fnt, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float font_size, float thick, Color col, Color shadow_color, float alpha , bool fill_all)
{
	Font _draw_font = fnt;
	if (!fill_all) {
		Draw_Text_Ex(fnt, text, x + thick, y, o_x, o_y, rot, spacing, font_size, shadow_color, alpha);
		Draw_Text_Ex(fnt, text, x, y + thick, o_x, o_y, rot, spacing, font_size, shadow_color, alpha);
	}
	else {
		for (int i = 1; i <= (int)(thick); i++) {
			Draw_Text_Ex(fnt, text, x + i, y, o_x, o_y, rot, spacing, font_size, shadow_color, alpha);
			Draw_Text_Ex(fnt, text, x, y + i, o_x, o_y, rot, spacing, font_size, shadow_color, alpha);
		}
	}
	Draw_Text_Ex(fnt, text, x, y, o_x, o_y, rot, spacing, font_size, col, alpha);
}
void F_Render::Draw_Text_Outline(F_Font fnt, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float font_size, float thick, Color col, Color outline_color, float alpha, bool fill_all)
{
	Draw_Text_Outline(fnt.to_raylib_font(), text, x, y, o_x, o_y, rot, spacing, font_size, thick, col, outline_color, alpha, fill_all);
}
void F_Render::Draw_Text_Shadow(F_Font fnt, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float font_size, float thick, Color col, Color shadow_color, float alpha, bool fill_all)
{
	Draw_Text_Shadow(fnt.to_raylib_font(), text, x, y, o_x, o_y, rot, spacing, font_size, thick, col, shadow_color, alpha, fill_all);
}
void F_Render::Draw_Text_Ex(F_Font font, const char* text, float x, float y, float o_x, float o_y, float rot, float spacing, float size, Color col, float alpha)
{
	Draw_Text_Ex(font.to_raylib_font(), text, x,y,o_x,o_y,rot,spacing,size,col, alpha);
}

extern Font f_default_font;
void F_Render::Draw_Text(const char* text, float x, float y, float o_x, float o_y, float rot, float size, Color col, float alpha)
{
	Draw_Text_Ex(f_default_font, text, x, y, o_x, o_y, rot, 0, size, col, alpha);
}
void F_Render::Set_Default_Font(Font font)
{
	 f_default_font = font;
}
void Load_FCamera( float w, float h, F_Camera* camera)
{
	camera->Load(w, h, true);
}

void Unload_FCamera(F_Camera* camera)
{
	camera->Unload();
}

void Begin_Mode_FCamera(F_Camera* _camera,Color Background_Color)
{
	BeginTextureMode(_camera->Get());
	ClearBackground(Background_Color);
}
void End_Mode_FCamera(F_Camera* _camera,Color Background_Color, int reverse)
{
	EndTextureMode();
	ClearBackground(Background_Color);
	DrawTexturePro(_camera->Get().texture,
		{ 0,0,_camera->Get_Width(),_camera->Get_Height()*(reverse?1:-1)},
		{ _camera->x,_camera->y,_camera->Get_Width() * _camera->x_scale,_camera->Get_Height() * _camera->y_scale },
		{ _camera->Get_Width() * _camera->x_scale * _camera->origin.x,
		_camera->Get_Height() * _camera->y_scale * _camera->origin.y }, _camera->angle, _camera->col);
}
void Begin_Mode_FCamera(F_Camera& _camera,Color Background_Color)
{
	BeginTextureMode(_camera.Get());
	ClearBackground(Background_Color);
}
void End_Mode_FCamera(F_Camera& _camera,Color Background_Color,int reverse)
{
	EndTextureMode();
	ClearBackground(Background_Color);
	DrawTexturePro(_camera.Get().texture,
		{ 0,0,_camera.Get_Width(),_camera.Get_Height() * (reverse ? 1 : -1) },
		{ _camera.x,_camera.y,_camera.Get_Width() * _camera.x_scale,_camera.Get_Height() * _camera.y_scale },
		{ _camera.Get_Width() * _camera.x_scale * _camera.origin.x,
		_camera.Get_Height() * _camera.y_scale * _camera.origin.y }, _camera.angle, _camera.col);
}

F_Camera::F_Camera()
{
	x_scale = y_scale = 1.0f;
	x = 0; y = 0;
	origin = { 0,0 };
	auto_free = true;
	angle = 0.0f;
	col = WHITE;
	width = height = 0.0f;
	camera_render = RenderTexture();
}

F_Camera::F_Camera(const F_Camera* c)
{
	x_scale = c->x_scale;
	y_scale = c->y_scale;
	x = c->x; y = c->y;
	origin = c->origin;
	camera_render = c->camera_render;
	auto_free = c->auto_free;
	angle = c->angle;
	col = c->col;
	width = c->width;
	height = c->height;
}

void F_Camera::Load(float w, float h,bool _auto_free)
{
	this->camera_render = LoadRenderTexture(w, h);
	auto_free = _auto_free;
	width = w;
	height = h;
}

void F_Camera::Unload()
{
	UnloadRenderTexture(this->camera_render);
}

float F_Camera::Get_Width() const
{
	return width;
}

float F_Camera::Get_Height() const
{
	return height;
}

RenderTexture F_Camera::Get()
{
	return camera_render;
}

F_Camera::~F_Camera()
{
	if (auto_free) {
		Unload();
	}
}

FilePathList F_File::drop_list = {};
bool F_File::loaded = 0;

struct FilterItem {
	std::string name;
	std::string extensions;
};

std::vector<nfdu8filteritem_t> ParseFilterString(const char* strFilter, std::vector<FilterItem>& outItems) {
	if (!strFilter || strFilter[0] == '\0') {
		return {};
	}

	std::stringstream ss(strFilter);
	std::string segment;
	std::vector<nfdu8filteritem_t> nfdFilters;
	while (std::getline(ss, segment, ';')) {
		if (segment.empty()) continue;

		std::string name = segment;

		// 尝试获取下一个 segment 作为 extensions
		if (std::getline(ss, segment, ';')) {
			std::string extensions = segment;

			// 填充内部结构体
			outItems.push_back({ name, extensions });

			nfdFilters.push_back({
				(const nfdu8char_t*)outItems.back().name.c_str(),
				(const nfdu8char_t*)outItems.back().extensions.c_str()
				});
		}
	}

	if (!outItems.empty()) {
		outItems.push_back({ "All Files", "*" });
		nfdFilters.push_back({
			(const nfdu8char_t*)outItems.back().name.c_str(),
			(const nfdu8char_t*)outItems.back().extensions.c_str()
			});
	}

	return nfdFilters;
}


static std::string execute_nfd_open(const char* strFilter) {
	nfdu8char_t* outPath = nullptr;

	std::vector<FilterItem> filterStrings;
	std::vector<nfdu8filteritem_t> nfdFilters = ParseFilterString(strFilter, filterStrings);

	// 2. 调用 NFD API
	nfdresult_t result = NFD_OpenDialog(
		&outPath,
		nfdFilters.empty() ? nullptr : nfdFilters.data(), 
		(nfdfiltersize_t)nfdFilters.size(),              
		nullptr                                        
	);

	
	if (result == NFD_OKAY) {
		std::string path((const char*)outPath);
		NFD_FreePath(outPath); 
		return path;
	}
	else if (result == NFD_CANCEL) {
		return "";
	}
	else {
		std::cerr << "NFD Open Dialog Error: " << NFD_GetError() << std::endl;
		return "";
	}
}

static std::string execute_nfd_save(const char* strFilter,const char* d_name) {
	nfdu8char_t* outPath = nullptr;
	
	
	std::vector<FilterItem> filterStrings;
	std::vector<nfdu8filteritem_t> nfdFilters = ParseFilterString(strFilter, filterStrings);

	
	nfdresult_t result = NFD_SaveDialog(
		&outPath,
		nfdFilters.empty() ? nullptr : nfdFilters.data(),
		(nfdfiltersize_t)nfdFilters.size(),
		nullptr,
		d_name
	);


	if (result == NFD_OKAY) {
		std::string path((const char*)outPath);
		NFD_FreePath(outPath); // 【关键修正】使用 NFD_FreePath
		return path;
	}
	else if (result == NFD_CANCEL) {
		return "";
	}
	else {
		std::cerr << "NFD Save Dialog Error: " << NFD_GetError() << std::endl;
		return "";
	}
}
std::string F_File::Get_Open_File_Name(const char* strFilter) {
	return execute_nfd_open(strFilter);
}



std::string F_File::Get_Save_File_Name(const char* strFilter, const char* d_name)
{
	return execute_nfd_save(strFilter, d_name);
}

void F_File::Flush_Drop_Files()
{
	if (IsFileDropped()) {
		drop_list = LoadDroppedFiles();
		loaded = true;
	}
}

const char* F_File::Get_Drop_File(int index)
{
	if (drop_list.count <= 0) {
		DEBUG_LOG(LOG_WARNING,"F_File_Get_Drop_File没有文件拖入",0);
	}
	else if (index >= drop_list.count) {
     	DEBUG_LOG(LOG_WARNING, "F_File_Get_Drop_File索引超出范围", 0);
	}
	else {
		return drop_list.paths[index];
	}
	return nullptr;
}

F_File::F_File()
{
	drop_list = FilePathList();
	loaded = false;
}

F_File::~F_File()
{
	if (loaded) {
		UnloadDroppedFiles(drop_list);
	}
}

Color F_Color::To_RlCol()
{
	return Make_Color_RGB(R, G, B, A);
}

F_Color::operator struct Color()
{
	return Make_Color_RGB(R, G, B, A);
}

#include <fstream>
// 字符串修剪辅助函数
static inline std::string& trim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
		}));
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
		}).base(), s.end());
	return s;
}

void F_Ini::parse_line(std::string& line) {
	line = line.substr(0, line.find(';'));  // 移除行内注释
	trim(line);

	if (line.empty()) return;

	if (line[0] == '[' && line.back() == ']') {
		currentSection = line.substr(1, line.size() - 2);
		trim(currentSection);
		sections[currentSection];  // 确保section存在
	}
	else if (auto eq_pos = line.find('='); eq_pos != std::string::npos) {
		std::string key = line.substr(0, eq_pos);
		std::string value = line.substr(eq_pos + 1);
		trim(key);
		trim(value);
		sections[currentSection][key] = value;
	}
}
F_Ini::F_Ini(const std::string& filename ) {
	if (!filename.empty()) {
		Load(filename);
	}
}

bool F_Ini::Load(const std::string& filename) {
	sections.clear();
	currentFile = filename;
	currentSection = "default";  // 默认section

	std::ifstream file(filename);
	if (!file.is_open()) return false;

	std::string line;
	while (std::getline(file, line)) {
		parse_line(line);
	}
	return true;
}

bool F_Ini::Save(const std::string& filename) {
	std::string save_path = filename.empty() ? currentFile : filename;
	if (save_path.empty()) return false;

	std::ofstream file(save_path);
	if (!file.is_open()) return false;

	for (const auto& [section, entries] : sections) {
		file << "[" << section << "]\n";
		for (const auto& [key, value] : entries) {
			file << key << "=" << value << "\n";
		}
		file << "\n";
	}
	return true;
}

// 读取方法
int F_Ini::Get_Int_Value_From_Name(const std::string& Class, const std::string& name) {
	try {
		return std::stoi(sections[Class][name]);
	}
	catch (...) {
		return 0;
	}
}

std::string F_Ini::Get_String_Value_From_Name(const std::string& Class, const std::string& name) {
	return sections[Class][name];
}

double F_Ini::Get_Double_Value_From_Name(const std::string& Class, const std::string& name) {
	try {
		return std::stod(sections[Class][name]);
	}
	catch (...) {
		return 0.0;
	}
}

// 写入方法
bool F_Ini::Write_Int_Value(const std::string& Class, const std::string& name, int value) {
	sections[Class][name] = std::to_string(value);
	return true;
}

bool F_Ini::Write_String_Value(const std::string& Class, const std::string& name, const std::string& value) {
	sections[Class][name] = value;
	return true;
}

bool F_Ini::Write_Double_Value(const std::string& Class, const std::string& name, double value) {
	sections[Class][name] = std::to_string(value);
	return true;
}
//json


namespace F_Json {

	class Json::iterator {
		using array_iterator_type = json::iterator;

		using object_iterator_type = json::iterator; 

		json* m_json_ptr;

		union {
			array_iterator_type array_it;
			object_iterator_type object_it;
		};
		bool is_object_iterator;

	public:
		using difference_type = std::ptrdiff_t;
		using value_type = Json;
		using pointer = Json*;
		using reference = Json&;
		using iterator_category = std::forward_iterator_tag;


		iterator(json* ptr, array_iterator_type it, bool is_object)
			: m_json_ptr(ptr), is_object_iterator(is_object)
		{
			if (is_object) new (&object_it) object_iterator_type(it);
			else new (&array_it) array_iterator_type(it);
		}

		value_type operator*() const {
			if (is_object_iterator) {
				return Json(std::make_shared<JsonValueImpl>(*object_it));
			}
			else {
				return Json(std::make_shared<JsonValueImpl>(*array_it));
			}
		}

		iterator& operator++() {
			if (is_object_iterator) ++object_it;
			else ++array_it;
			return *this;
		}

		bool operator==(const iterator& other) const {
			if (is_object_iterator) return object_it == other.object_it;
			return array_it == other.array_it;
		}
		bool operator!=(const iterator& other) const { return !(*this == other); }
	};//迭代器
	
	class Json::const_iterator {
		using nlohmann_const_iterator = json::const_iterator;

		const json* m_json_ptr;

		union {
			nlohmann_const_iterator array_it;
			nlohmann_const_iterator object_it; 
		};
		bool is_object_iterator;

	public:
		using difference_type = std::ptrdiff_t;
		using value_type = Json;
		using pointer = Json*;
		using reference = Json&;
		using iterator_category = std::forward_iterator_tag;

		const_iterator(const json* ptr, nlohmann_const_iterator it, bool is_object)
			: m_json_ptr(ptr), is_object_iterator(is_object)
		{
			if (is_object) new (&object_it) nlohmann_const_iterator(it);
			else new (&array_it) nlohmann_const_iterator(it);
		}

		value_type operator*() const {
			if (is_object_iterator) {
				return Json(std::make_shared<JsonValueImpl>(*object_it)); 
			}
			else {
				return Json(std::make_shared<JsonValueImpl>(*array_it));
			}
		}

		const_iterator& operator++() {
			if (is_object_iterator) ++object_it;
			else ++array_it;
			return *this;
		}

		bool operator==(const const_iterator& other) const {
			if (is_object_iterator) return object_it == other.object_it;
			return array_it == other.array_it;
		}
		bool operator!=(const const_iterator& other) const { return !(*this == other); }
	};//const迭代器


	using nlohmann::json;
	

	JsonType to_json_type(const json& j) {
		if (j.is_null()) return JsonType::Null;
		if (j.is_boolean()) return JsonType::Boolean;
		if (j.is_number()) return JsonType::Number;
		if (j.is_string()) return JsonType::String;
		if (j.is_array()) return JsonType::Array;
		if (j.is_object()) return JsonType::Object;
		throw std::runtime_error("Unknown JSON type in conversion.");
	}

	Json::Json(std::shared_ptr<JsonValueImpl> v) : val(std::move(v)) {}

	Json::Json() : val(std::make_shared<JsonValueImpl>()) {
	}
	Json::Json(std::initializer_list<Json> list) {
		val = std::make_shared<JsonValueImpl>(json::array());
		for (const auto& item : list) {
			val->data.push_back(item.val->data);
		}
	}

	Json::Json(std::initializer_list<std::pair<const std::string, Json>> list) {	
		val = std::make_shared<JsonValueImpl>(json::object());
		for (const auto& pair : list) {
			val->data[pair.first] = pair.second.val->data;
		}
	}
	
	const JsonValueImpl& Json::getInternalValue() const {
		return *val;
	}
	Json::iterator Json::begin() {
		json* ptr = &val->data;

		if (val->data.is_array()) {
			return iterator(ptr, ptr->begin(), false);
		}
		else if (val->data.is_object()) {
			return iterator(ptr, ptr->begin(), true);
		}
		else {
			return end();
		}
	}

	Json::iterator Json::end() {
		json* ptr = &val->data;

		if (val->data.is_array()) {
			return iterator(ptr, ptr->end(), false);
		}
		else {
			return iterator(ptr, ptr->end(), true); 
		}
	}
	Json::const_iterator Json::cbegin() const {
		const json* ptr = &val->data;

		if (val->data.is_array()) {
			return const_iterator(ptr, ptr->cbegin(), false);
		}
		else if (val->data.is_object()) {
			return const_iterator(ptr, ptr->cbegin(), true);
		}
		else {
			return cend();
		}
	}

	Json::const_iterator Json::cend() const {
		const json* ptr = &val->data;

		if (val->data.is_array()) {
			return const_iterator(ptr, ptr->cend(), false);
		}
		else {
			return const_iterator(ptr, ptr->cend(), true);
		}
	}

	// --- 工厂方法 ---
	Json Json::Null() { return Json(); }
	Json Json::Boolean(bool b) { return Json(std::make_shared<JsonValueImpl>(json(b))); }
	Json Json::Number(double d) { return Json(std::make_shared<JsonValueImpl>(json(d))); }
	Json Json::String(const std::string& s) { return Json(std::make_shared<JsonValueImpl>(json(s))); }
	Json Json::Array() { return Json(std::make_shared<JsonValueImpl>(json::array())); }
	Json Json::Object() { return Json(std::make_shared<JsonValueImpl>(json::object())); }



	Json& Json::operator=(double n) { val->data = n; return *this; }
	Json& Json::operator=(const char* str) { val->data = str; return *this; }
	Json& Json::operator=(const std::string& s) { val->data = s; return *this; }
	Json& Json::operator=(bool b) { val->data = b; return *this; }
	Json& Json::operator=(const Json& j) {
		val = std::make_shared<JsonValueImpl>(j.val->data);
		return *this;
	}

	JsonType Json::type() const { return to_json_type(val->data); }
	bool Json::isNull() const { return val->data.is_null(); }
	bool Json::has(const std::string& key) const { return val->data.contains(key); }
	size_t Json::size() const { return val->data.size(); }

	bool Json::asBoolean() const { return val->data.get<bool>(); }
	double Json::asNumber() const { return val->data.get<double>(); }
	std::string Json::asString() const { return val->data.get<std::string>(); }
	Json& Json::add(const Json& item) {
		if (!val->data.is_array()) throw std::logic_error("Json::add() on non-array type.");
		val->data.push_back(item.val->data);
		return *this;
	}

	Json Json::get(size_t index) const {
		if (!val->data.is_array()) throw std::logic_error("Json::get(index) on non-array type.");
		if (index >= val->data.size()) return Json::Null();
		return Json(std::make_shared<JsonValueImpl>(val->data.at(index)));
	}

	Json Json::operator[](size_t index) const { return get(index); }

	Json& Json::set(const std::string& key, const Json& v) {
		if (!val->data.is_object()) throw std::logic_error("Json::set() on non-object type.");
		val->data[key] = v.val->data;
		return *this;
	}

	Json Json::get(const std::string& key) const {
		if (!val->data.is_object()) return Json::Null();
		if (!val->data.contains(key)) return Json::Null();
		return Json(std::make_shared<JsonValueImpl>(val->data.at(key)));
	}

	Json Json::operator[](const std::string& key) const { return get(key); }


	// --- 序列化和解析 ---
	std::string Json::serialize() const {
		return val->data.dump(-1, ' ', val->data.is_array() ? false : true); // -1 = 紧凑模式
	}

	std::string Json::serialize_with_format() const {
		return val->data.dump(4); // 4 = 4个空格缩进
	}

	Json Json::parse(const std::string& s) {
		try {
			json parsedData = json::parse(s);
			return Json(std::make_shared<JsonValueImpl>(parsedData));
		}
		catch (const json::parse_error& e) {
			std::stringstream ss;
			ss << "JSON Parse Failed: " << e.what() << " at position " << e.byte;
			throw std::invalid_argument(ss.str());
		}
	}

} // namespace F_Json


//lua
F_Lua::F_Lua() {
	// 初始化Lua状态，打开所有标准库
	lua_.open_libraries(
		sol::lib::base,
		sol::lib::package,
		sol::lib::coroutine,
		sol::lib::string,
		sol::lib::os,
		sol::lib::math,
		sol::lib::table,
		sol::lib::debug,
		sol::lib::bit32,
		sol::lib::io,
		sol::lib::utf8
	);

	// 设置错误处理
	lua_.set_exception_handler([](lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description) {
		std::cerr << "Lua异常: ";
		if (maybe_exception) {
			std::cerr << maybe_exception->what() << " - ";
		}
		std::cerr << description << std::endl;
		return sol::stack::push(L, description);
		});
}

F_Lua::~F_Lua() {
	// sol::state会自动清理资源
}

bool F_Lua::ExecuteScript(const std::string& filepath) {
	try {
		auto result = lua_.script_file(filepath);
		return result.valid();
	}
	catch (const sol::error& e) {
		std::cerr << "执行脚本错误: " << e.what() << std::endl;
		return false;
	}
}

bool F_Lua::ExecuteString(const std::string& code) {
	try {
		auto result = lua_.script(code, sol::script_pass_on_error);
		if (!result.valid()) {
			sol::error err = result;
			std::cerr << "Lua代码执行错误: " << err.what() << std::endl;
			return false;
		}
		return true;
	}
	catch (const sol::error& e) {
		std::cerr << "执行字符串错误: " << e.what() << std::endl;
		return false;
	}
}

sol::protected_function_result F_Lua::ExecuteStringSafe(const std::string& code) {
	return lua_.script(code, sol::script_pass_on_error);
}

void F_Lua::HandleError(const sol::protected_function_result& result) {
	if (!result.valid()) {
		sol::error err = result;
		std::cerr << "Lua错误: " << err.what() << std::endl;
	}
}




// 全局状态
static std::map<std::string, F_Gui::CustomWidgetFunc> customWidgets;
static ImGuiStyle defaultStyle;

namespace F_Gui {
	void Init() {
		rlImGuiSetup(true);  // 启用深色主题
		defaultStyle = ImGui::GetStyle();
	}

	void Shutdown() {
		rlImGuiShutdown();
	}

	void BeginFrame() {
		rlImGuiBegin();
	}

	void EndFrame() {
		rlImGuiEnd();
	}

	// 窗口管理
	void BeginWindow(const char* title, bool* p_open, int flags) {
		ImGui::Begin(title, p_open, flags);
	}

	void EndWindow() {
		ImGui::End();
	}

	// 基础控件
	bool Button(const char* label, const Vector2& size) {
		return ImGui::Button(label, ImVec2(size.x, size.y));
	}

	void Text(const char* fmt, ...) {
		va_list args;
		va_start(args, fmt);
		ImGui::TextV(fmt, args);
		va_end(args);
	}

	bool Checkbox(const char* label, bool* v) {
		return ImGui::Checkbox(label, v);
	}

	bool SliderFloat(const char* label, float* v, float v_min, float v_max) {
		return ImGui::SliderFloat(label, v, v_min, v_max);
	}

	bool ColorEdit3(const char* label, float col[3]) {
		return ImGui::ColorEdit3(label, col);
	}

	// 布局控件
	void BeginGroup(const char* id) {
		ImGui::BeginGroup();
		ImGui::PushID(id);
	}

	void EndGroup() {
		ImGui::PopID();
		ImGui::EndGroup();
	}

	void SameLine(float offset_x, float spacing) {
		ImGui::SameLine(offset_x, spacing);
	}

	// 高级控件
	bool TreeNode(const char* label) {
		return ImGui::TreeNode(label);
	}

	void TreePop() {
		ImGui::TreePop();
	}

	void PlotLines(const char* label, const float* values, int values_count,
		float scale_min, float scale_max, Vector2 size) {
		ImGui::PlotLines(label, values, values_count, 0, NULL, scale_min, scale_max, ImVec2(size.x, size.y));
	}

	// 游戏引擎专用组件
	void Vector3Control(const char* label, Vector3& values, float resetValue) {
		ImGui::PushID(label);
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text("%s", label);
		ImGui::NextColumn();

		ImGuiStyle& style = ImGui::GetStyle();
		float totalWidth = ImGui::CalcItemWidth();
		float itemWidth = (totalWidth - style.ItemSpacing.x * 2) / 3.0f;
		float buttonSize = ImGui::GetFontSize() + style.FramePadding.y * 2.0f;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });

		// X
		ImGui::PushItemWidth(itemWidth);
		if (ImGui::Button("X", ImVec2{ buttonSize, buttonSize })) values.x = resetValue;
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// Y
		ImGui::PushItemWidth(itemWidth);
		if (ImGui::Button("Y", ImVec2{ buttonSize, buttonSize })) values.y = resetValue;
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// Z
		ImGui::PushItemWidth(itemWidth);
		if (ImGui::Button("Z", ImVec2{ buttonSize, buttonSize })) values.z = resetValue;
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}

	void TexturePreview(Texture2D texture, Vector2 size) {
		rlImGuiImageSize(&texture, size.x, size.y);
	}

	void FrameRateOverlay() {
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos;
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = work_pos.x + work_size.x - PAD;
		window_pos.y = work_pos.y + PAD;
		window_pos_pivot.x = 1.0f;
		window_pos_pivot.y = 0.0f;

		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowBgAlpha(0.35f);

		if (ImGui::Begin("FPS Overlay", NULL,
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_NoNav))
		{
			ImGui::Text("FPS: %.1f", GetFPS());
			ImGui::Text("Frame: %.3f ms", GetFrameTime() * 1000.0f);
			ImGui::Separator();
			ImGui::Text("Renderer: width=%.1f,height=%.1f,name:%s",  GetRenderWidth(),GetRenderHeight(),GetMonitorName(GetCurrentMonitor()));
		}
		ImGui::End();
	}

	// 样式系统
	void PushStyleColor(Color idx, Color color) {
		ImColor i = ImColor((int)color.r, (int)color.g, (int)color.b, (int)color.a);
		ImGui::PushStyleColor(i, ImVec4(
			color.r / 255.0f,
			color.g / 255.0f,
			color.b / 255.0f,
			color.a / 255.0f
		));
	}

	void PopStyleColor(int count) {
		ImGui::PopStyleColor(count);
	}

	void LoadTheme(const char* name) {
		if (strcmp(name, "dark") == 0) {
			ImGui::StyleColorsDark();
		}
		else if (strcmp(name, "light") == 0) {
			ImGui::StyleColorsLight();
		}
		else if (strcmp(name, "classic") == 0) {
			ImGui::StyleColorsClassic();
		}
		else {
			ImGui::GetStyle() = defaultStyle;
		}
	}

	// 自定义绘制
	void DrawLine(const Vector2& start, const Vector2& end, Color color, float thickness) {
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddLine(
			ImVec2(start.x, start.y),
			ImVec2(end.x, end.y),
			IM_COL32(color.r, color.g, color.b, color.a),
			thickness
		);
	}

	void DrawRect(const rlRectangle& rect, Color color, float rounding) {
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddRect(
			ImVec2(rect.x, rect.y),
			ImVec2(rect.x + rect.width, rect.y + rect.height),
			IM_COL32(color.r, color.g, color.b, color.a),
			rounding
		);
	}

	// 扩展系统
	void RegisterWidget(const char* name, CustomWidgetFunc func) {
		customWidgets[name] = func;
	}

	void ShowWidget(const char* name) {
		if (customWidgets.find(name) != customWidgets.end()) {
			customWidgets[name]();
		}
	}
}
