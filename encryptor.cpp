#include <bits/stdc++.h>

// 平台相关的头文件
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#elif defined(__APPLE__) || defined(__linux__)
#include <termios.h>
#include <unistd.h>
#endif

// 跨平台的暂停函数
void cross_platform_pause() {
#ifdef _WIN32
    system("pause");
#else
    std::cout << "按回车键继续...";
    std::cin.get();
#endif
}

// 跨平台的控制台UTF-8设置
void setup_console_utf8() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#elif defined(__APPLE__)
    // macOS 默认支持 UTF-8，通常不需要特殊设置
    setlocale(LC_ALL, "en_US.UTF-8");
#elif defined(__linux__)
    setlocale(LC_ALL, "en_US.UTF-8");
#endif
}

void strip_quotes(std::string& s) {
    if (not empty(s) and s.front() == '"' and s.back() == '"') {
        s = s.substr(1, ssize(s) - 2);
    }
    return ;
}

int main() {
    // 设置控制台UTF-8支持
    setup_console_utf8();

    std::cout << "========= 文件加密/解密工具 =========\n";
    std::cout << "说明：此程序可对任意文件进行简单加密/解密。\n";
#ifdef _WIN32
    std::cout << "      文件路径可直接拖入此窗口，然后回车。\n";
#else
    std::cout << "      请输入完整的文件路径。\n";
#endif
    std::cout << "====================================\n\n";

    std::string file_name; {
#ifdef _WIN32
        std::cout << "请将要加密/解密的文件拖入此窗口，并按回车：";
#else
        std::cout << "请输入要加密/解密的文件路径：";
#endif
        std::getline(std::cin, file_name);
        strip_quotes(file_name);
    }

    std::ifstream ifs(file_name, std::ios::binary); {
        if (not ifs) {
            std::cout << "无法打开文件: " << file_name << "\n";
            cross_platform_pause();
            return 1;
        }
    }

    std::string key{}; {
        std::cout << "请输入密钥: ";
        std::getline(std::cin, key);
        if (key.empty()) {
            std::cout << "\n【错误】密钥不能为空!\n";
            cross_platform_pause();
            return 1;
        }
    }

    std::cout << "请选择操作类型: 1.加密  2.解密  (输入数字1或2): ";
    int op{}; {
        std::string input_op; {
            std::getline(std::cin, input_op);
        }
        try {
            op = std::stoi(input_op);
        } catch (...) {
            op = 0;
        }
        if (op != 1 and op != 2) {
            std::cout << "\n【错误】操作类型输入无效!\n";
            cross_platform_pause();
            return 1;
        }
    }

    std::string output_name;
    if (op == 1) {
        auto file_name_without_suf = file_name; [&] {
            auto it = file_name.find_last_of('.');
            if (it == file_name.npos) {return ;}
            file_name_without_suf = file_name.substr(0, it);
        }();
        output_name = file_name_without_suf + ".enc";
    } else {
        // 解密时如果原文件名以 .enc 结尾则去掉，否则加 .dec
        if ((file_name.size() >= 4 and file_name.substr(file_name.size() - 4) == ".enc")) {
            output_name = file_name.substr(0, file_name.size() - 4);
        } else if ([&] {
            auto it = file_name.find_last_of('.');
            return (it != file_name.npos);
        }()) {
            auto it = file_name.find_last_of('.');
            output_name = file_name.substr(0, it) + ".dec";
        } else {
            output_name = file_name + ".dec";
        }
    }

    std::ofstream outfile(output_name, std::ios::binary);
    if (!outfile) {
        std::cout << "\n【错误】无法创建输出文件: " << output_name << "\n";
        cross_platform_pause();
        return 1;
    }

    // 获取文件大小用于显示进度
    ifs.seekg(0, std::ios::end);
    auto filesize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::cout << "\n正在处理文件, 请稍候...\n";
    char ch;
    auto keylen = key.length();
    auto i = 0U, read_bytes = 0U, last_percent = 0U;
    
    while (ifs.get(ch)) {
        ch ^= key[i % keylen];
        outfile.put(ch);
        ++i; ++read_bytes;

        // 显示进度
        if (filesize > 0) {
            std::size_t percent = (read_bytes * 100 / filesize);
            if (percent != last_percent and percent % 5 == 0) {
                std::cout << "\r进度: " << percent << "%   " << std::flush;
                last_percent = percent;
            }
        }
    }
    std::cout << "\r进度: 100%   " << std::endl;

    ifs.close();
    outfile.close();

    std::cout << "\n\n处理完成!\n";
    std::cout << "输出文件: " << output_name << "\n";
    std::cout << "请检查文件是否可正常使用。\n";
    std::cout << "------------------------------------\n";
    
    cross_platform_pause();
    return 0;
}
