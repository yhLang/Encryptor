#include <bits/stdc++.h>
#include <windows.h>

void strip_quotes(std::string& s) {
    if (not empty(s) and s.front() == '"' and s.back() == '"') {
        s = s.substr(1, ssize(s) - 2);
    }
    return ;
}

int main() {

    SetConsoleOutputCP(CP_UTF8);

    std::cout << "========= 文件加密/解密工具 =========\n";
    std::cout << "说明：此程序可对任意文件进行简单加密/解密。\n";
    std::cout << "      文件路径可直接拖入此窗口，然后回车。\n";
    std::cout << "====================================\n\n";

    std::string file_name; {
        std::cout << "请将要加密/解密的文件拖入此窗口，并按回车：";
        std::getline(std::cin, file_name);
        strip_quotes(file_name);
    }

    std::ifstream ifs(file_name, std::ios::binary); {
        if (not ifs) {
            std::cout << "无法打开文件\n";
            system("pause");
            return 1;
        }
    }

    std::string key{}; {
        std::cout << "请输入密钥";
        std::getline(std::cin, key);
    }

    std::cout << "请选择操作类型: 1.加密  2.解密  (输入数字1或2)" << "\n";
    int op{}; {
        std::string input_op; {
            std::cin >> input_op;
        }
        try {
            op = std::stoi(input_op);
        } catch (...) {
            op = 0;
        }
        if (op != 1 and op != 2) {
            std::cout << "\n【错误】操作类型输入无效!\n";
            system("pause");
            return 1;
        }
    }

    std::string output_name;
    if (op == 1) {
        output_name = file_name + ".enc";
    } else {
        // 解密时如果原文件名以 .enc 结尾则去掉，否则加 .dec
        if ((file_name.size() >= 4 and file_name.substr(file_name.size() - 4) == ".enc")) {
            output_name = file_name.substr(0, file_name.size() - 4);
        } else if ([&] {
            auto it = file_name.find_last_of('.');
            return (it != file_name.npos);
        }()) {
            auto it = file_name.find_last_of('.');
            auto del = ssize(file_name) - it;
            output_name = file_name.substr(0, file_name.size() - del);
        } else {
            output_name = file_name + ".dec";
        }
    }

    std::ofstream ofs(output_name, std::ios::binary); {
        if (not ofs) {
            std::cout << "\n无法创建输出文件" << output_name << "\n";
            system("pause");
            return 1;
        }
    }

    std::ofstream outfile(output_name, std::ios::binary);
    if (!outfile) {
        std::cout << "\n【错误】无法创建输出文件" << output_name << "\n";
        system("pause");
        return 1;
    }

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

        if (filesize > 0) {
            std::size_t percent = (read_bytes * 100 / filesize);
            if (percent != last_percent and percent % 5 == 0) {
                std::cout << "\r" << percent << "%   " << std::flush;
                last_percent = percent;
            }
        }
    }
    std::cout << "\r100%   " << std::endl;

    ifs.close();
    outfile.close();

    std::cout << "\n\n处理完成!\n输出文件已生成\n" << output_name << "\n";
    std::cout << "请检查文件是否可正常使用。\n";
    std::cout << "------------------------------------\n";
    std::cout << "（按任意键退出）";
    system("pause");
    return 0;

    return 0;
}