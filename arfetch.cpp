#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <algorithm>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <pwd.h>
#include <cstdio>

namespace fs = std::filesystem;
using namespace std;


const string RESET   = "\033[0m";
const string BOLD    = "\033[1m";
const string CYAN    = "\033[36m";
const string MAGENTA = "\033[35m";
const string YELLOW  = "\033[33m";
const string GREEN   = "\033[32m";
const string BLUE    = "\033[34m";
const string RED     = "\033[31m";


string getGpuInfo() {
    string gpu = "Desconocida";
    const string path = "/sys/class/drm/";
    if (fs::exists(path)) {
        for (const auto& entry : fs::directory_iterator(path)) {
            string uevent_path = entry.path().string() + "/device/uevent";
            ifstream file(uevent_path);
            if (file.is_open()) {
                string line;
                while (getline(file, line)) {
                    if (line.find("PCI_ID=") != string::npos) {
                        if (line.find("8086") != string::npos) return "Intel Graphics";
                        if (line.find("10DE") != string::npos) return "NVIDIA GPU";
                        if (line.find("1002") != string::npos) return "AMD Radeon";
                    }
                }
            }
        }
    }
    return gpu;
}


string getDistroInfo() {
    ifstream file("/etc/os-release");
    string line, name = "Linux";
    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.find("PRETTY_NAME=") == 0) {
                name = line.substr(13);
                name.erase(std::remove(name.begin(), name.end(), '\"'), name.end());
            }
        }
    }
    return name;
}


vector<string> getLogo() {
    return {
        "",
        "  /\\___/\\",
        "  )     (",
        " =\\     /=",
        "   )   (",
        "  /     \\",
        "  )     (",
        " /       \\",
        " \\       /",
        "  \\__ __/",
        "    ))",
        "   //",
        "  ((",
        "   \\)",
    };
}

string getUptime() {
    struct sysinfo info;
    if (sysinfo(&info) != 0) return "N/A";
    long s = info.uptime;
    int d = s / 86400, h = (s % 86400) / 3600, m = (s % 3600) / 60;
    stringstream ss;
    if (d > 0) ss << d << "d ";
    if (h > 0) ss << h << "h ";
    ss << m << "m";
    return ss.str();
}

string getCpuInfo() {
    ifstream file("/proc/cpuinfo");
    string line;
    while (getline(file, line)) {
        if (line.find("model name") == 0) return line.substr(line.find(':') + 2);
    }
    return "Unknown";
}

pair<double, double> getMemoryInfo() {
    ifstream file("/proc/meminfo");
    long total = 0, avail = 0;
    string key; long val;
    while (file >> key >> val) {
        if (key == "MemTotal:") total = val;
        else if (key == "MemAvailable:") avail = val;
        string tmp; getline(file, tmp);
    }
    double totalGB = (double)total / 1048576.0;
    double usage = (total > 0) ? 100.0 * (total - avail) / total : 0.0;
    return {totalGB, usage};
}

string runCmd(const string& cmd) {
    string result;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "";
    char buf[256];
    while (fgets(buf, sizeof(buf), pipe)) result += buf;
    pclose(pipe);
    return result;
}

string getPackages() {
    struct { const char* bin; const char* cmd; } managers[] = {
        {"/usr/bin/xbps-query", "xbps-query -l | wc -l"},
        {"/usr/bin/dpkg-query", "dpkg-query -f '.\n' -W | wc -l"},
        {"/usr/bin/pacman",     "pacman -Qq | wc -l"},
        {"/usr/bin/rpm",        "rpm -qa | wc -l"},
        {"/usr/bin/qlist",      "qlist -I | wc -l"},
    };
    for (const auto& m : managers) {
        if (fs::exists(m.bin)) {
            string out = runCmd(m.cmd);
            out.erase(std::remove(out.begin(), out.end(), '\n'), out.end());
            out.erase(std::remove(out.begin(), out.end(), ' '), out.end());
            if (!out.empty()) return out + " (pkgs)";
        }
    }
    return "N/A";
}

string getBattery() {
    string path = "/sys/class/power_supply/BAT0/";
    if (!fs::exists(path)) path = "/sys/class/power_supply/BAT1/";
    if (fs::exists(path)) {
        ifstream cap(path + "capacity");
        ifstream stat(path + "status");
        string c, s;
        if (cap >> c && stat >> s) {
            return c + "% (" + s + ")";
        }
    }
    return "No detectada";
}


int main() {
    struct utsname sys;
    uname(&sys);
    char host[256];
    gethostname(host, sizeof(host));
    string user = getenv("USER") ? getenv("USER") : "user";
    string distro = getDistroInfo();

    const auto now = chrono::system_clock::now();
    const time_t now_time = chrono::system_clock::to_time_t(now);
    tm* local_tm = localtime(&now_time);

    auto [ram_total, ram_usage] = getMemoryInfo();

    vector<string> info;
    info.push_back("  " + MAGENTA + BOLD + user + RESET + "@" + BOLD + host + RESET);
    info.push_back("  " + MAGENTA + "--------------------------" + RESET);
    info.push_back(YELLOW  + "  OS       : " + RESET + distro);
    info.push_back(YELLOW  + "  Kernel   : " + RESET + sys.release);
    info.push_back(GREEN   + "  Uptime   : " + RESET + getUptime());
    {
        ostringstream ss;
        ss << put_time(local_tm, "%Y-%m-%d %H:%M");
        info.push_back(GREEN + "  Date     : " + RESET + ss.str());
    }
    info.push_back(CYAN    + "  Packages : " + RESET + getPackages());
    {
        ostringstream ss;
        ss << fixed << setprecision(1) << ram_usage << "% (" << setprecision(2) << ram_total << " GiB)";
        info.push_back(MAGENTA + "  Memory   : " + RESET + ss.str());
    }
    info.push_back(BLUE    + "  CPU      : " + RESET + getCpuInfo());
    info.push_back(BLUE    + "  GPU      : " + RESET + getGpuInfo());
    info.push_back(RED     + "  Battery  : " + RESET + getBattery());
    info.push_back(BLUE    + "  Shell    : " + RESET + (getenv("SHELL") ? getenv("SHELL") : "sh"));
    info.push_back("");
    {
        string pal = "  ";
        for (int i = 40; i <= 47; i++) pal += "\033[" + to_string(i) + "m   ";
        pal += RESET;
        info.push_back(pal);
    }

    vector<string> logo = getLogo();
    const size_t logo_width = 12;

    cout << endl;
    for (size_t i = 0; i < max(logo.size(), info.size()); i++) {
        string line = (i < logo.size()) ? logo[i] : "";
        while (line.size() < logo_width) line += " ";
        cout << CYAN << line << RESET;
        if (i < info.size()) cout << info[i];
        cout << endl;
    }
    cout << endl;

    return 0;
}
