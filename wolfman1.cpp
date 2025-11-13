#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>

using namespace std;

enum Identity{
	WEREWOLF,
	VILLAGER,
    SEER,
	WITCH,
	HUNTER,
	GUARDIAN,
};
struct IdentityInfo{
	Identity id;
	string name;
	int count;
};
struct Player {
    int number;
    Identity identity;
    string identityName;
};
string getIdentityName(Identity id) {
    switch(id) {
        case WEREWOLF: return "狼人";
        case VILLAGER: return "村民";
        case SEER: return "预言家";
        case WITCH: return "女巫";
        case HUNTER: return "猎人";
        case GUARDIAN: return "守卫";
    }
}
vector<IdentityInfo> initIdentityConfig(int totalPlayers) {
    vector<IdentityInfo> config;
    
    // 根据总人数设置默认身份配置
    int wolfCount, seerCount = 1, witchCount = 1, hunterCount = 1;
    
    if (totalPlayers <= 4) {
        wolfCount = 1;
    } else if (totalPlayers <= 6) {
        wolfCount = 2;
    } else if (totalPlayers <= 8) {
        wolfCount = 3;
    } else if (totalPlayers <= 11) {
        wolfCount = 4;
    } else {
        wolfCount = 5;
    }
    
    // 村民数量 = 总人数 - 其他身份数量
    int villagerCount = totalPlayers - (wolfCount + seerCount + witchCount + hunterCount);
    
    // 确保村民数量不为负
    if (villagerCount < 0) {
        villagerCount = 0;
        // 如果村民数量为负，减少白痴数量
        
    }
    
    config.push_back({WEREWOLF, "狼人", wolfCount});
    config.push_back({VILLAGER, "村民", villagerCount});
    config.push_back({SEER, "预言家", seerCount});
    config.push_back({WITCH, "女巫", witchCount});
    config.push_back({HUNTER, "猎人", hunterCount});
    
    
    return config;
}
vector<Player> assignIdentities(int totalPlayers, const vector<IdentityInfo>& config) {
    vector<Player> players;
    vector<Identity> identityPool;
    
    // 填充身份池
    for (const auto& info : config) {
        for (int i = 0; i < info.count; ++i) {
            identityPool.push_back(info.id);
        }
    }
    
    // 使用当前时间作为随机种子
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(identityPool.begin(), identityPool.end(), default_random_engine(seed));
    
    // 分配身份给玩家
    for (int i = 0; i < totalPlayers; ++i) {
        Player player;
        player.number = i + 1;  // 玩家编号从1开始
        player.identity = identityPool[i];
        player.identityName = getIdentityName(identityPool[i]);
        players.push_back(player);
    }
    
    return players;
}
void displayConfig(const vector<IdentityInfo>& config) {
    cout << "\n身份配置:" << endl;
    for (const auto& info : config) {
        if (info.count > 0) {
            cout << info.name << ":" << info.count << "人" << endl;
        }
    }
}

// 显示玩家身份（供管理员查看）
void displayAllIdentities(const vector<Player>& players) {
    cout << "\n所有玩家身份:" << endl;
    for (const auto& player : players) {
        cout << "玩家" << player.number << ":" << player.identityName << endl;
    }
}

// 显示单个玩家的身份（供玩家自己查看）
void displayPlayerIdentity(const vector<Player>& players, int playerNumber) {
    for (const auto& player : players) {
        if (player.number == playerNumber) {
            cout << "\n你(玩家" << player.number << ")的身份是：" << player.identityName << endl;
            return;
        }
    }
    cout << "\n无效的玩家编号!" << endl;
}
int main(){
	cout << "-----狼人杀玩家身份分配-----"<< endl;
	int totalPlayers;
	cout << "请输入玩家人数(建议为5-15人):" << endl;
	cin >> totalPlayers;
    if (totalPlayers < 2){
		cout << "玩家人数不能少于2人" << endl;
		return 1;
	}
	vector<IdentityInfo> config = initIdentityConfig(totalPlayers);
	displayConfig(config);
	 // 随机分配身份
    vector<Player> players = assignIdentities(totalPlayers, config);
    
    // 显示所有身份（供法官/管理员查看）
    char showAll;
    cout << "\n是否显示所有玩家身份?(y/n,仅法官查看):";
    cin >> showAll;
    if (showAll == 'y' || showAll == 'Y') {
        displayAllIdentities(players);
    }
	 // 玩家查询自己的身份
    while (true) {
        int playerNum;
        cout << "\n请输入你的玩家编号(1-" << totalPlayers << ",输入0退出):";
        cin >> playerNum;
        
        if (playerNum == 0) {
            break;
        }
        
        if (playerNum >= 1 && playerNum <= totalPlayers) {
            displayPlayerIdentity(players, playerNum);
        } else {
            cout << "请输入有效的玩家编号！" << endl;
        }
    }
    
    cout << "\n游戏愉快!" << endl;
    return 0;
}