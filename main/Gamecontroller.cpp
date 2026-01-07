#include "GameController.h"
#include <iostream>
#include <random>
#include <algorithm>

namespace WerewolfGame {

    // 构造函数
    GameController::GameController()
        : _current_phase(GamePhase::DAY), _game_over(false), _winning_team(Team::VILLAGERS),
          _tonight_werewolf_target(-1), _tonight_healed_player(-1), _tonight_poisoned_player(-1),
          if_antidote_used(false), if_poison_used(false) {
    }
    
    // 初始化游戏
    void GameController::initializeGame(int player_count, const std::vector<std::string>& player_names) {
        _players.clear();
        _seer_check_records.clear();
        _game_over = false;
        _current_phase = GamePhase::DAY;
        
        // 重置夜晚状态
        _tonight_werewolf_target = -1;
        _tonight_healed_player = -1;
        _tonight_poisoned_player = -1;
        if_antidote_used = false;
        if_poison_used = false;

        // 创建玩家
        for (int i = 0; i < player_count && i < static_cast<int>(player_names.size()); ++i) {
            _players.emplace_back(i + 1, player_names[i], i > 0);
        }
        
        // 分配角色
        assignRoles();
        std::cout << "Game initialized with " << player_count << " players." << std::endl;
    }
    
    // ========== 玩家信息查询 ==========
    
    const Player& GameController::getPlayer(int player_id) const {
        if (player_id < 1 || player_id > static_cast<int>(_players.size())) {
            throw std::out_of_range("Invalid player ID");
        }
        return _players[player_id - 1];
    }
    
    bool GameController::isPlayerAlive(int player_id) const {
        for (const auto& player : _players) {
            if (player.id == player_id && player.status == PlayerStatus::ALIVE) {
                return true;
            }
        }
        return false;
    }
    
    std::vector<int> GameController::getAlivePlayers() const {
        std::vector<int> alive;
        for (const auto& player : _players) {
            if (player.status == PlayerStatus::ALIVE) {
                alive.push_back(player.id);
            }
        }
        return alive;
    }
    
    void GameController::eliminatePlayer(int player_id) {
        for (auto& player : _players) {
            if (player.id == player_id) {
                player.status = PlayerStatus::DEAD;
                std::cout << "Player " << player_id << " has been eliminated." << std::endl;
                break;
            }
        }
    }
    
    // ========== 预言家相关 ==========
    
    bool GameController::hasPlayerBeenChecked(int player_id) const {
        for (const auto& check_record : _seer_check_records) {
            if (check_record.target_id == player_id) {
                return true;
            }
        }
        return false;
    }
    
    void GameController::performSeerCheck(int seer_id, int target_id) {
        // 验证目标玩家
        if (target_id <= 0 || target_id > static_cast<int>(_players.size())) {
            std::cout << "Invalid player ID." << std::endl;
            return;
        }

        const auto& target = _players[target_id - 1];
        if (target.status != PlayerStatus::ALIVE) {
            std::cout << "Player " << target_id << " is dead." << std::endl;
            return;
        }

        if (hasPlayerBeenChecked(target_id)) {
            std::cout << "Player " << target_id << " has already been checked." << std::endl;
            return;
        }
        
        // 记录查验结果
        _seer_check_records.push_back(SeerCheckRecord(target_id, target.role));
        
        std::cout << "Seer (Player " << seer_id << ") checked Player " << target_id 
                  << ": Role is " << (target.role == RoleType::WEREWOLF ? "WEREWOLF" : "GOOD") << "." << std::endl;
    }
    
    // ========== 女巫相关 ==========
    
    void GameController::useAntidote(int player_id) {
        _tonight_healed_player = player_id;
        if_antidote_used = true;
        std::cout << "Witch used antidote on Player " << player_id << std::endl;
    }
    
    void GameController::usePoison(int player_id) {
        _tonight_poisoned_player = player_id;
        if_poison_used = true;
        std::cout << "Witch used poison on Player " << player_id << std::endl;
    }
    
    // ========== 游戏状态管理 ==========
    
    void GameController::checkGameOver() {
        int alive_villagers = 0;
        int alive_werewolves = 0;

        for (const auto& player : _players) {
            if (player.status == PlayerStatus::ALIVE) {
                if (player.team == Team::VILLAGERS) {
                    alive_villagers++;
                } else {
                    alive_werewolves++;
                }
            }
        }

        if (alive_werewolves == 0) {
            _game_over = true;
            _winning_team = Team::VILLAGERS;
            std::cout << "\n=== GAME OVER ===" << std::endl;
            std::cout << "Villagers win! All werewolves have been eliminated." << std::endl;
        } else if (alive_werewolves >= alive_villagers) {
            _game_over = true;
            _winning_team = Team::WEREWOLVES;
            std::cout << "\n=== GAME OVER ===" << std::endl;
            std::cout << "Werewolves win! They outnumber the villagers." << std::endl;
        }
    }
    
    void GameController::printPlayers() const {
        std::cout << "\n=== PLAYER LIST ===" << std::endl;
        for (const auto& player : _players) {
            std::string role_name = "Unknown";
            switch (player.role) {
                case RoleType::VILLAGER: role_name = "Villager"; break;
                case RoleType::WEREWOLF: role_name = "Werewolf"; break;
                case RoleType::SEER: role_name = "Seer"; break;
                case RoleType::WITCH: role_name = "Witch"; break;
                case RoleType::HUNTER: role_name = "Hunter"; break;
            }

            std::cout << "Player " << player.id << ": " << player.name
                      << " [" << role_name << "] "
                      << (player.status == PlayerStatus::ALIVE ? "(Alive)" : "(Dead)") << std::endl;
        }
    }
    
    // ========== 角色分配（保留原有逻辑）==========
    
    void GameController::assignRoles() {
        std::vector<RoleType> roles = generateRoleSetup(static_cast<int>(_players.size()));
        for (size_t i = 0; i < _players.size() && i < roles.size(); ++i) {
            _players[i].role = roles[i];
            _players[i].team = (roles[i] == RoleType::WEREWOLF ? Team::WEREWOLVES : Team::VILLAGERS);
        }
    }
    
    std::vector<RoleType> GameController::generateRoleSetup(int player_count) const {
        std::vector<RoleType> roles;

        int werewolf_count = GameConstants::DEFAULT_WEREWOLF_COUNT;
        int villager_count = GameConstants::DEFAULT_VILLAGER_COUNT;

        if (player_count > 8) {
            werewolf_count = 3;
            villager_count = player_count - werewolf_count - 3;
        }
        //仅用来测试二人版
        if (player_count == 2){
            werewolf_count = 1;
            villager_count = 1;
        }

        for (int i = 0; i < werewolf_count; ++i) roles.push_back(RoleType::WEREWOLF);
        for (int i = 0; i < villager_count; ++i) roles.push_back(RoleType::VILLAGER);
        roles.push_back(RoleType::SEER);
        roles.push_back(RoleType::WITCH);
        roles.push_back(RoleType::HUNTER);

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(roles.begin(), roles.end(), g);

        return roles;
    }

} // namespace WerewolfGame
