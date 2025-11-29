#include "FloatApi.h"
#include <thread>
#include <mutex>
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

void Audio_Play(F_Audio audio) {
	AudioMode am = audio.GetAudioMode();
	switch (am) {
	case AudioMode::Audio_Unknown:
		DEBUG_LOG(LOG_WARNING, TextFormat("Audio_Play:未初始化的audio!"), false);
		break;
	case AudioMode::Audio_Sound:
		PlaySound(audio.GetSound());
		break;
	case AudioMode::Audio_Music:
		PlayMusicStream(audio.GetMusic());
		break;
	default:
		DEBUG_LOG(LOG_ERROR, "F_Audio:错误的audio_mode,请明确");
		break;
	}
}
void Audio_Stop(F_Audio audio) {
	AudioMode am = audio.GetAudioMode();
	switch (am) {
	case AudioMode::Audio_Unknown:
		DEBUG_LOG(LOG_WARNING, TextFormat("Audio_Play:未初始化的audio!"), false);
		break;
	case AudioMode::Audio_Sound:
		StopSound(audio.GetSound());
		break;
	case AudioMode::Audio_Music:
		StopMusicStream(audio.GetMusic());
		break;
	default:
		DEBUG_LOG(LOG_ERROR, "F_Audio:错误的audio_mode,请明确");
		break;
	}
}
void Audio_Music_Update(F_Audio audio) {
	if (audio.IsTheAudioMode(AudioMode::Audio_Music))
		UpdateMusicStream(audio.GetMusic());
	else {
		DEBUG_LOG(LOG_ERROR, "Audio_Music_Update:只能对music进行update!", false);
	}
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

#include <string.h>
using namespace WinFuns;
typedef struct tagOFNA {
	DWORD         lStructSize;
	void*         hwndOwner;
	void*     hInstance;
	const char*       lpstrFilter;
	const char*        lpstrCustomFilter;
	DWORD         nMaxCustFilter;
	DWORD         nFilterIndex;
	char*        lpstrFile;
	DWORD         nMaxFile;
	char*        lpstrFileTitle;
	DWORD         nMaxFileTitle;
	const char*       lpstrInitialDir;
	const char*    lpstrTitle;
	DWORD         Flags;
	WORD          nFileOffset;
	WORD          nFileExtension;
	const char*       lpstrDefExt;
	long unsigned*        lCustData;
	const char*       lpTemplateName;
	const char*        lpstrPrompt;
	void* pvReserved;
	DWORD         dwReserved;
	DWORD         FlagsEx;
} OPENFILENAMEA, * LPOPENFILENAMEA;
typedef struct tagOFNW {
	DWORD         lStructSize;
	void* hwndOwner;
	void* hInstance;
	const wchar_t* lpstrFilter;
	const wchar_t* lpstrCustomFilter;
	DWORD         nMaxCustFilter;
	DWORD         nFilterIndex;
	wchar_t* lpstrFile;
	DWORD         nMaxFile;
	wchar_t* lpstrFileTitle;
	DWORD         nMaxFileTitle;
	const wchar_t* lpstrInitialDir;
	const wchar_t* lpstrTitle;
	DWORD         Flags;
	WORD          nFileOffset;
	WORD          nFileExtension;
	const wchar_t* lpstrDefExt;
	long unsigned* lCustData;
	const wchar_t* lpTemplateName;
	const wchar_t* lpstrPrompt;
	void* pvReserved;
	DWORD         dwReserved;
	DWORD         FlagsEx;
} OPENFILENAMEW, * LPOPENFILENAMEW;
#define OPENFILENAME OPENFILENAMEA
extern "C" int GetOpenFileNameA(OPENFILENAME* ofn);
extern "C" int GetOpenFileNameW(OPENFILENAMEW* ofn);
extern "C" int GetSaveFileNameA(OPENFILENAME* ofn);
extern "C" int GetSaveFileNameW(OPENFILENAMEW* ofn);
#define TCHAR wchar_t
std::string F_File::Get_Open_File_Name(const char*strFilter)
{
	using namespace WinFuns;
	OPENFILENAME ofn;
	char szFile[260] = { 0 };

	// 初始化OPENFILENAME结构体
	memset(&ofn, sizeof(ofn),0);
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = strFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.hwndOwner = GetWindowHandle();
	ofn.Flags = 0x00000004 | 0x00000800 | 0x00001000 | 0x00100000;

	// 显示文件对话框
	if (GetOpenFileNameA(&ofn) == 1) {
		return std::string(ofn.lpstrFile);
	}
	else {
		return "";  // 用户取消选择或发生错误
	}
}
bool F_File::loaded = false;
FilePathList F_File::drop_list = FilePathList();
#include <fstream>
std::string F_File::Get_Open_File_Name(const char* strFilter,unsigned int flag)
{
	// 初始化 OPENFILENAME 结构体
	OPENFILENAMEA ofn;
	char szFile[260] = ""; // 文件名缓冲区
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetWindowHandle(); // 如果是窗口程序，可传入窗口句柄
	ofn.lpstrFile = szFile;  // 将文件名缓冲区绑定到结构体
	ofn.nMaxFile = sizeof(szFile);
	// 设置文件过滤器
	ofn.lpstrFilter = strFilter;
	ofn.nFilterIndex = 1;
	ofn.Flags = 0x00000800 | 0x00001000;

	// 打开“打开文件”对话框
	if (GetOpenFileNameA(&ofn)) {
		std::ifstream file(ofn.lpstrFile);
		if (file.is_open()) {
			file.close();
			return ofn.lpstrFile;
		}
		else {
			std::cerr << "Failed to open file: " << ofn.lpstrFile << std::endl;
		}
	}
	else {
		std::cerr << "Open file dialog canceled or failed." << std::endl;
	}
	return "";
}

std::wstring F_File::Get_Open_File_NameW(const wchar_t* strFilter)
{
	using namespace WinFuns;
	OPENFILENAMEW ofn;
	wchar_t szFile[260] = { 0 };

	// 初始化OPENFILENAME结构体
	memset(&ofn, sizeof(ofn), 0);
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = strFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.hwndOwner = GetWindowHandle();
	ofn.Flags = 0x00000004 | 0x00000800 | 0x00001000 | 0x00100000;
	// 显示文件对话框
	if (GetOpenFileNameW(&ofn) == 1) {
		return std::wstring(ofn.lpstrFile);
	}
	else {
		return L"";  // 用户取消选择或发生错误
	}
}

std::wstring F_File::Get_Open_File_NameW(const wchar_t* strFilter, unsigned int flag)
{
	using namespace WinFuns;
	OPENFILENAMEW ofn;
	wchar_t szFile[260] = { 0 };

	// 初始化OPENFILENAME结构体
	memset(&ofn, sizeof(ofn), 0);
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = (strFilter);
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.hwndOwner = GetWindowHandle();
	ofn.Flags = flag;
	// 显示文件对话框
	if (GetOpenFileNameW(&ofn) == 1) {
		return std::wstring(ofn.lpstrFile);
	}
	else {
		return L"";  // 用户取消选择或发生错误
	}
}

std::string F_File::Get_Save_File_Name(const char* strFilter)
{
	using namespace WinFuns;
	OPENFILENAME ofn;
	char szFile[260] = { 0 };

	// 初始化OPENFILENAME结构体
	memset(&ofn, sizeof(ofn), 0);
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = strFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.hwndOwner = GetWindowHandle();
	ofn.Flags = 0x00000004 | 0x00000800 | 0x00001000 | 0x00100000;

	// 显示文件对话框
	if (GetSaveFileNameA(&ofn) == 1) {
		return std::string(ofn.lpstrFile);
	}
	else {
		return "";  // 用户取消选择或发生错误
	}
}

std::string F_File::Get_Save_File_Name(const char* strFilter, unsigned int flag)
{
	using namespace WinFuns;
	OPENFILENAME ofn;
	char szFile[260] = { 0 };

	// 初始化OPENFILENAME结构体
	memset(&ofn, sizeof(ofn), 0);
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = (strFilter);
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.hwndOwner = GetWindowHandle();
	ofn.Flags = flag;
	// 显示文件对话框
	if (GetSaveFileNameA(&ofn) == 1) {
		return std::string(ofn.lpstrFile);
	}
	else {
		return "";  // 用户取消选择或发生错误
	}
}

std::wstring F_File::Get_Save_File_NameW(const wchar_t* strFilter)
{
	using namespace WinFuns;
	OPENFILENAMEW ofn;
	wchar_t szFile[260] = { 0 };

	// 初始化OPENFILENAME结构体
	memset(&ofn, sizeof(ofn), 0);
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = (strFilter);
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.hwndOwner = GetWindowHandle();
	ofn.Flags = 0x00000004 | 0x00000800 | 0x00001000 | 0x00100000;
	// 显示文件对话框
	if (GetOpenFileNameW(&ofn) == 1) {
		return std::wstring(ofn.lpstrFile);
	}
	else {
		return L"";  // 用户取消选择或发生错误
	}
}

std::wstring F_File::Get_Save_File_NameW(const wchar_t* strFilter, unsigned int flag)
{
	using namespace WinFuns;
	OPENFILENAMEW ofn;
	wchar_t szFile[260] = { 0 };

	// 初始化OPENFILENAME结构体
	memset(&ofn, sizeof(ofn), 0);
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = (strFilter);
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.hwndOwner = GetWindowHandle();
	ofn.Flags = flag;
	// 显示文件对话框
	if (GetOpenFileNameW(&ofn) == 1) {
		return std::wstring(ofn.lpstrFile);
	}
	else {
		return L"";  // 用户取消选择或发生错误
	}
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
#include <sstream>
#include <cctype>
#include <algorithm>

namespace F_Json {

	// JsonNull 实现
	JsonType JsonNull::type() const { return JsonType::Null; }
	std::string JsonNull::serialize(bool no_name) const { return "null"; }

	// JsonBoolean 实现
	JsonBoolean::JsonBoolean(bool v) : val(v) {}
	JsonType JsonBoolean::type() const { return JsonType::Boolean; }
	std::string JsonBoolean::serialize(bool no_name) const { return val ? "true" : "false"; }
	bool JsonBoolean::value() const { return val; }

	// JsonNumber 实现
	JsonNumber::JsonNumber(double v) : val(v) {}
	JsonType JsonNumber::type() const { return JsonType::Number; }
	std::string JsonNumber::serialize(bool no_name) const {
		std::ostringstream oss;
		if (val == static_cast<int>(val)) {
			oss << static_cast<int>(val);
		}
		else {
			oss << val;
		}
		return oss.str();
	}
	double JsonNumber::value() const { return val; }

	// JsonString 实现
	JsonString::JsonString(std::string v) : val(std::move(v)) {}
	JsonType JsonString::type() const { return JsonType::String; }
	std::string JsonString::serialize(bool no_name) const {
		std::ostringstream oss;
		oss << '"';
		for (char c : val) {
			switch (c) {
			case '"':  oss << "\\\""; break;
			case '\\': oss << "\\\\"; break;
			case '\b': oss << "\\b";  break;
			case '\f': oss << "\\f";  break;
			case '\n': oss << "\\n";  break;
			case '\r': oss << "\\r";  break;
			case '\t': oss << "\\t";  break;
			default:   oss << c;      break;
			}
		}
		oss << '"';
		return oss.str();
	}
	std::string JsonString::value() const { return val; }

	// JsonArray 实现
	JsonType JsonArray::type() const { return JsonType::Array; }
	std::string JsonArray::serialize(bool no_name) const {
		std::string res = "[";
		for (size_t i = 0; i < vals.size(); ++i) {
			if (i > 0) res += ", ";
			res += vals[i]->serialize();
		}
		return res + "]";
	}
	void JsonArray::add(std::shared_ptr<JsonValue> v) { vals.push_back(v); }
	size_t JsonArray::size() const { return vals.size(); }
	std::shared_ptr<JsonValue> JsonArray::get(size_t i) const { return vals.at(i); }

	// JsonObject 实现
	JsonType JsonObject::type() const { return JsonType::Object; }
	std::string JsonObject::serialize(bool no_name) const {
		std::string res = "{";
		bool first = true;
		for (const auto& [k, v] : vals) {
			if (!first) res += ", ";
			if(no_name==0)
				res += JsonString(k).serialize() + ": " + v->serialize();
			else res+= v->serialize();
			first = false;
		}
		return res + "}";
	}
	void JsonObject::set(const std::string& k, std::shared_ptr<JsonValue> v) { vals[k] = v; }
	bool JsonObject::has(const std::string& k) const { return vals.count(k); }
	std::shared_ptr<JsonValue> JsonObject::get(const std::string& k) const { return vals.at(k); }

	size_t JsonObject::size() const {
		return vals.size();
	}

	// Json 包装类实现
	Json::Json(std::shared_ptr<JsonValue> v) : val(v) {}
	Json::Json() : val(std::make_shared<JsonNull>()) {}

	Json Json::Null() { return Json(std::make_shared<JsonNull>()); }
	Json Json::Boolean(bool b) { return Json(std::make_shared<JsonBoolean>(b)); }
	Json Json::Number(double d) { return Json(std::make_shared<JsonNumber>(d)); }
	Json Json::String(const std::string& s) { return Json(std::make_shared<JsonString>(s)); }
	Json Json::Array() { return Json(std::make_shared<JsonArray>()); }
	Json Json::Object() { return Json(std::make_shared<JsonObject>()); }

	JsonType Json::type() const { return val->type(); }

	bool Json::asBoolean() const {
		auto p = dynamic_cast<JsonBoolean*>(val.get());
		if (!p) throw std::runtime_error("Not a boolean");
		return p->value();
	}

	double Json::asNumber() const {
		auto p = dynamic_cast<JsonNumber*>(val.get());
		if (!p) throw std::runtime_error("Not a number");
		return p->value();
	}

	void Json::setNoName()
	{
		no_name = 1;
	}

	void Json::setHasName()
	{
		no_name = 0;
	}

	std::string Json::asString() const {
		auto p = dynamic_cast<JsonString*>(val.get());
		if (!p) throw std::runtime_error("Not a string");
		return p->value();
	}

	Json& Json::add(const Json& item) {
		if (val->type() != JsonType::Array) {
			throw std::runtime_error("Not an array");
		}
		auto p = static_cast<JsonArray*>(val.get());
		p->add(item.val);
		return *this;
	}

	Json Json::get(size_t index) const {
		auto p = dynamic_cast<JsonArray*>(val.get());
		if (!p) throw std::runtime_error("Not an array");
		return Json(p->get(index));
	}

	Json Json::operator[](size_t index) const
	{
		return get(index);
	}

	Json& Json::set(const std::string& key, const Json& v) {
		auto p = dynamic_cast<JsonObject*>(val.get());
		if (!p) throw std::runtime_error("Not an object");
		p->set(key, v.val);
		return *this;
	}
	Json Json::get(const std::string& key) const {
		auto p = dynamic_cast<JsonObject*>(val.get());
		if (!p) throw std::runtime_error("Not an object");
		return Json(p->get(key));
	}

	Json Json::operator[](const std::string& key) const
	{
		return get(key);
	}

	std::string Json::serialize() const { return val->serialize(no_name); }

	std::string Json::serialize_with_format() const
	{
		std::string json = val->serialize(no_name);
		std::string formatted;
		int indent_level = 0;
		bool in_quotes = false;

		for (char c : json) {
			if (c == '\"') in_quotes = !in_quotes;

			if (!in_quotes) {
				if (c == '{' ) {
					formatted += "{\n";
					indent_level++;
					formatted += std::string(indent_level, '\t');
				}
				else if (c == '}') {
					formatted += "\n";
					indent_level--;
					formatted += std::string(indent_level, '\t') + "}";
				}
				else if (c == '[') {
					formatted += "[\n";
					indent_level++;
					formatted += std::string(indent_level, '\t');
				}
				else if (c == ']') {
					formatted += "\n";
					indent_level--;
					formatted += std::string(indent_level, '\t') + "]";
				}
				else if (c == ',') {
					formatted += ",\n" + std::string(indent_level, '\t');
				}
				else if (c == ':') {
					formatted += ": ";
				}
				else if (c == ' ') {
					continue;
				}
				else {
					formatted += c;
				}
			}
			else {
				formatted += c;
			}
		}
		return formatted;
		
	}

	// 解析相关辅助函数
	namespace {
		// f_json.cpp
		static void skip_ws(const std::string& s, size_t& pos) {
			while (pos < s.size()) {
				// 处理单行注释
				if (s[pos] == '/' && pos + 1 < s.size() && s[pos + 1] == '/') {
					pos += 2;
					while (pos < s.size() && s[pos] != '\n') pos++;
					continue;
				}

				// 处理多行注释
				if (s[pos] == '/' && pos + 1 < s.size() && s[pos + 1] == '*') {
					pos += 2;
					while (pos < s.size()) {
						if (s[pos] == '*' && pos + 1 < s.size() && s[pos + 1] == '/') {
							pos += 2;
							break;
						}
						pos++;
					}
					continue;
				}

				// 空白处理
				if (!std::isspace(s[pos])) break;
				pos++;
			}
		}

		Json parse_value(const std::string& s, size_t& pos);
		Json parse_string(const std::string& s, size_t& pos);
		
		Json parse_object(const std::string& s, size_t& pos) {
			Json obj = Json::Object();
			pos++; // Skip '{'
			bool expect_comma = false; // 标记是否期望逗号

			while (pos < s.size()) {
				skip_ws(s, pos);

				// 检查对象结束
				if (s[pos] == '}') {
					pos++;
					return obj;
				}

				// 检查逗号（如果是期望的）
				if (expect_comma) {
					if (s[pos] == ',') {
						pos++;
						skip_ws(s, pos);
					}
					else {
						throw std::runtime_error("Expected ',' or '}'");
					}
				}

				// 解析键值对
				auto key = parse_string(s, pos).asString();
				skip_ws(s, pos);

				if (s[pos++] != ':') {
					throw std::runtime_error("Expected ':'");
				}

				auto val = parse_value(s, pos);
				obj.set(key, val);
				expect_comma = true; // 下一个应期望逗号或结束符
			}

			throw std::runtime_error("Unterminated object");
		}

		Json parse_array(const std::string& s, size_t& pos) {
			Json arr = Json::Array();
			pos++; // Skip '['
			while (pos < s.size()) {
				skip_ws(s, pos);
				if (s[pos] == ']') { pos++; break; }

				arr.add(parse_value(s, pos));
				skip_ws(s, pos);
				if (s[pos] == ',') pos++;
				else if (s[pos] != ']') throw std::runtime_error("Expected ',' or ']'");
			}
			return arr;
		}

		static Json parse_string(const std::string& s, size_t& pos) {
			std::string res;
			bool escape = false;
			pos++; // Skip opening "
			while (pos < s.size()) {
				char c = s[pos++];
				if (escape) {
					switch (c) {
					case '"':  res += '"';  break;
					case '\\': res += '\\'; break;
					case 'b':  res += '\b'; break;
					case 'f':  res += '\f'; break;
					case 'n':  res += '\n'; break;
					case 'r':  res += '\r'; break;
					case 't':  res += '\t'; break;
					default:   throw std::runtime_error("Invalid escape");
					}
					escape = false;
				}
				else if (c == '"') {
					return Json::String(res);
				}
				else if (c == '\\') {
					escape = true;
				}
				else {
					res += c;
				}
			}
			throw std::runtime_error("Unterminated string");
		}

		Json parse_bool(const std::string& s, size_t& pos) {
			if (s.substr(pos, 4) == "true") {
				pos += 4;
				return Json::Boolean(true);
			}
			else if (s.substr(pos, 5) == "false") {
				pos += 5;
				return Json::Boolean(false);
			}
			throw std::runtime_error("Invalid boolean");
		}

		Json parse_null(const std::string& s, size_t& pos) {
			if (s.substr(pos, 4) == "null") {
				pos += 4;
				return Json::Null();
			}
			throw std::runtime_error("Invalid null");
		}

		Json parse_number(const std::string& s, size_t& pos) {
			size_t end = pos;
			if (s[end] == '-') end++;
			while (end < s.size() && std::isdigit(s[end])) end++;
			if (s[end] == '.') end++;
			while (end < s.size() && std::isdigit(s[end])) end++;
			if (s[end] == 'e' || s[end] == 'E') {
				end++;
				if (s[end] == '+' || s[end] == '-') end++;
				while (end < s.size() && std::isdigit(s[end])) end++;
			}
			double num = std::stod(s.substr(pos, end - pos));
			pos = end;
			return Json::Number(num);
		}

		Json parse_value(const std::string& s, size_t& pos) {
			skip_ws(s, pos);
			if (pos >= s.size()) throw std::runtime_error("Unexpected end");

			switch (s[pos]) {
			case '{': return parse_object(s, pos);
			case '[': return parse_array(s, pos);
			case '"': return parse_string(s, pos);
			case 't': case 'f': return parse_bool(s, pos);
			case 'n': return parse_null(s, pos);
			default:  return parse_number(s, pos);
			}
		}
	}

	Json Json::parse(const std::string& s) {
		size_t pos = 0;
		return parse_value(s, pos);
	}
	bool Json::has(const std::string& key) const {
		if (auto obj = dynamic_cast<JsonObject*>(val.get())) {
			return obj->has(key);
		}
		throw std::runtime_error("Not an object");
	}

	bool Json::isNull() const {
		return val->type() == JsonType::Null;
	}

	size_t Json::size() const {
		if (auto arr = dynamic_cast<JsonArray*>(val.get())) {
			return arr->size();
		}
		if (auto obj = dynamic_cast<JsonObject*>(val.get())) {
			return obj->size();
		}
		throw std::runtime_error("Not a container type");
	}

	Json& Json::operator=(double n)
	{
		val = std::make_shared<JsonNumber>(n);
		return *this;
	}
	Json& Json::operator=(const std::string& s)
	{
		val = std::make_shared<JsonString>(s);
		return *this;
	}
	Json& Json::operator=(const char* s)
	{
		val = std::make_shared<JsonString>(s);
		return *this;
	}
	Json& Json::operator=(bool b)
	{
		val = std::make_shared<JsonBoolean>(b);
		return *this;
	}
	Json& Json::operator=(const Json& j)
	{
		val = j.val;
		return *this;
	}
}

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

	void DrawRect(const Rectangle& rect, Color color, float rounding) {
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

F_NetWork::F_NetWork() {
}

F_NetWork::~F_NetWork() {
	Shutdown();
}

bool F_NetWork::Initialize() {
	ClearError();
	int a = enet_initialize();
	if(a != 0) {
		DEBUG_LOG(LOG_ERROR,"ENet initialization failed!");
		SetError(NetErrorCode::INIT_FAILED, a,
			"ENet初始化失败",
			"请检查网络库依赖是否正确安装");
		return false;
	}
	return true;
}
std::string F_NetWork::GetEnetErrorString(int errorCode) {
	switch (errorCode) {
	case 0: return "成功";
	case 1: return "不兼容的协议版本";
	case 2: return "内存不足";
	case 3: return "无效参数";
	case 4: return "对等连接数已达上限";
	case 5: return "连接失败";
	case 6: return "已经连接";
	case 7: return "未连接";
	case 8: return "连接丢失";
	case 9: return "超时";
	case 10: return "数据包太大";
	case 11: return "在错误的线程调用";
	default: return "未知错误";
	}
}

// 辅助函数：设置错误信息
void F_NetWork::SetError(NetErrorCode code, int enetError,
	const std::string& message,
	const std::string& details) {
	lastError.code = code;
	lastError.enetError = enetError;
	lastError.message = message;
	lastError.details = details;

	// 输出到日志
	std::stringstream ss;
	ss << "网络错误 [" << static_cast<int>(code) << "]: "
		<< message << " (ENet: " << enetError << " - "
		<< GetEnetErrorString(enetError) << ")";
	if (!details.empty()) {
		ss << "\n详情: " << details;
	}

	TraceLog(LOG_ERROR, "%s", ss.str().c_str());
}
bool F_NetWork::CreateServer(int port, int maxClients) {
	ClearError();
	if (host) Shutdown(); // 关闭现有连接

	ENetAddress address;
	address.host = ENET_HOST_ANY;
	address.port = port;
	
	host = enet_host_create(&address, maxClients, 2, 0, 0);
	if (!host) {
		std::cerr << "Failed to create server on port " << port << std::endl;
		int enetErr = WinFuns::WSAGetLastError();
		SetError(NetErrorCode::SERVER_CREATE_FAILED, enetErr,
			"服务器创建失败",
			"端口: " + std::to_string(port) +
			", 最大客户端: " + std::to_string(maxClients));
		return false;
	}

	mode = NetMode::SERVER;
	std::cout << "Server started on port " << port << std::endl;
	return true;
}

bool F_NetWork::ConnectToServer(const char* hostAddress, int port, int timeout) {
	if (host) Shutdown(); // 关闭现有连接

	host = enet_host_create(nullptr, 1, 2, 0, 0);
	if (!host) {
		std::cerr << "Failed to create client" << std::endl;
		return false;
	}
	
	ENetAddress address;
	enet_address_set_host(&address, hostAddress);
	address.port = port;

	ENetPeer* peer = enet_host_connect(host, &address, 2, 0);
	if (!peer) {
		std::cerr << "Failed to connect to " << hostAddress << ":" << port << std::endl;
		return false;
	}

	// 等待连接建立
	ENetEvent event;
	if (enet_host_service(host, &event, timeout) > 0 &&
		event.type == ENET_EVENT_TYPE_CONNECT) {
		mode = NetMode::CLIENT;
		std::cout << "Connected to server " << hostAddress << ":" << port << std::endl;
		return true;
	}

	enet_peer_reset(peer);
	std::cerr << "Connection timeout to " << hostAddress << ":" << port << std::endl;
	return false;
}

void F_NetWork::Shutdown() {
	if (host) {
		enet_host_destroy(host);
		host = nullptr;
	}
	mode = NetMode::NONE;
}

void F_NetWork::SendPacket(ENetPeer* peer, const void* data, size_t dataLength,
	bool reliable, uint8_t channel) {
	if (!peer || !data || dataLength == 0) return;

	ENetPacket* packet = enet_packet_create(
		data,
		dataLength,
		reliable ? ENET_PACKET_FLAG_RELIABLE : ENET_PACKET_FLAG_UNSEQUENCED
	);

	enet_peer_send(peer, channel, packet);
}

void F_NetWork::BroadcastPacket(const void* data, size_t dataLength,
	bool reliable, uint8_t channel) {
	if (!host || mode != NetMode::SERVER) return;

	ENetPacket* packet = enet_packet_create(
		data,
		dataLength,
		reliable ? ENET_PACKET_FLAG_RELIABLE : ENET_PACKET_FLAG_UNSEQUENCED
	);

	enet_host_broadcast(host, channel, packet);
}

NetEvent F_NetWork::Service(int timeout) {
	NetEvent netEvent;
	if (!host) return netEvent;

	ENetEvent enetEvent;
	if (enet_host_service(host, &enetEvent, timeout) <= 0) {
		return netEvent; // 无事件
	}

	// 转换事件类型
	switch (enetEvent.type) {
	case ENET_EVENT_TYPE_CONNECT:
		netEvent.type = NetEventType::CONNECT;
		netEvent.peer = enetEvent.peer;
		break;

	case ENET_EVENT_TYPE_DISCONNECT:
		netEvent.type = NetEventType::DISCONNECT;
		netEvent.peer = enetEvent.peer;
		netEvent.data = enetEvent.data;
		break;

	case ENET_EVENT_TYPE_RECEIVE:
		netEvent.type = NetEventType::RECEIVE;
		netEvent.peer = enetEvent.peer;
		netEvent.packet = enetEvent.packet;
		break;

	default:
		break;
	}

	// 调用自定义回调
	if (eventCallback) {
		eventCallback(netEvent);
	}

	return netEvent;
}

bool F_NetWork::ConnectToServerDomain(const std::string& domain, int port, int timeout) {
	// 解析域名
	DNSResult result = ResolveDomain(domain);

	if (result.error) {
		std::cerr << "DNS resolution failed for " << domain
			<< ": " << result.error.message() << std::endl;
		return false;
	}

	if (result.ip_addresses.empty()) {
		std::cerr << "No IP addresses found for domain: " << domain << std::endl;
		return false;
	}

	std::cout << "Resolved " << domain << " to IPs: ";
	for (const auto& ip : result.ip_addresses) {
		std::cout << ip << " ";
	}
	std::cout << std::endl;

	// 尝试连接所有解析出的IP地址
	for (const auto& ip : result.ip_addresses) {
		std::cout << "Trying to connect to " << ip << ":" << port << std::endl;
		if (ConnectToServer(ip.c_str(), port, timeout)) {
			lastConnectedIP = ip; // 记录成功连接的IP
			return true;
		}
	}

	std::cerr << "Failed to connect to any IP address for domain: " << domain << std::endl;
	return false;
}

DNSResult F_NetWork::ResolveDomain(const std::string& domain, int timeout_ms) {
	DNSResult result;
	result.hostname = domain;

	// 使用互斥锁和条件变量实现超时机制
	std::mutex mtx;
	std::condition_variable cv;
	bool resolved = false;
	std::string resolved_ip;

	// 在独立线程中执行解析
	std::thread resolver([&]() {
		ENetAddress address;

		// 使用ENet内置的域名解析
		if (enet_address_set_host(&address, domain.c_str()) == 0) {
			// 将解析出的IP地址转换为字符串
			char ip_str[16] = { 0 }; // 足够存放IPv4地址
			enet_address_get_host_ip(&address, ip_str, sizeof(ip_str));
			resolved_ip = ip_str;
		}

		// 标记解析完成
		{
			std::lock_guard<std::mutex> lock(mtx);
			resolved = true;
		}
		cv.notify_one();
		});

	// 设置线程为分离状态（防止资源泄漏）
	resolver.detach();

	// 等待解析完成或超时
	std::unique_lock<std::mutex> lock(mtx);
	if (cv.wait_for(lock, std::chrono::milliseconds(timeout_ms), [&] { return resolved; })) {
		if (!resolved_ip.empty()) {
			result.ip_addresses.push_back(resolved_ip);
		}
		else {
			result.error = std::make_error_code(std::errc::address_not_available);
		}
	}
	else {
		// 超时处理
		result.error = std::make_error_code(std::errc::timed_out);
	}

	return result;
}