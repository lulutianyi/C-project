#pragma once
#include "GameTypes.h"
#include <vector>
#include <string>
#include <unordered_map>

namespace WerewolfGame {

    class GameController {
    public:
        GameController();

        // 游戏初始化（保留）
        void initializeGame(int player_count, const std::vector<std::string>& player_names);

        // ========== 新增：供状态机使用的接口 ==========
        
        // 玩家信息查询
        const Player& getPlayer(int player_id) const;
        bool isPlayerAlive(int player_id) const;
        std::vector<int> getAlivePlayers() const;
        void eliminatePlayer(int player_id);
        
        // 狼人相关
        int getWerewolfTarget() const { return _tonight_werewolf_target; }
        void setWerewolfTarget(int target) { _tonight_werewolf_target = target; }
        
        // 预言家相关
        bool hasPlayerBeenChecked(int player_id) const;
        void performSeerCheck(int seer_id, int target_id);
        
        // 女巫相关
        bool isAntidoteUsed() const { return if_antidote_used; }
        bool isPoisonUsed() const { return if_poison_used; }
        void useAntidote(int player_id);
        void usePoison(int player_id);
        int getHealedPlayer() const { return _tonight_healed_player; }
        int getPoisonedPlayer() const { return _tonight_poisoned_player; }
        
        // 游戏状态查询
        bool isGameOver() const { return _game_over; }
        GamePhase getCurrentPhase() const { return _current_phase; }
        Team getWinningTeam() const { return _winning_team; }
        const std::vector<Player>& getPlayers() const { return _players; }
        
        // 游戏状态管理
        void checkGameOver();
        void printPlayers() const;

    private:
        // 角色分配（保留）
        void assignRoles();
        std::vector<RoleType> generateRoleSetup(int player_count) const;

        // 预言家查验记录
        struct SeerCheckRecord {
            int target_id;
            RoleType role;
            SeerCheckRecord(int id, RoleType r) : target_id(id), role(r) {}
        };
        std::vector<SeerCheckRecord> _seer_check_records;

        // ========== 游戏状态数据 ==========
        
        // 玩家数据
        std::vector<Player> _players;
        
        // 当前阶段
        GamePhase _current_phase;
        
        // 游戏结束标志
        bool _game_over;
        Team _winning_team;
        
        // 夜晚行动数据
        int _tonight_werewolf_target;   // 狼人今晚选择的目标
        int _tonight_healed_player;     // 女巫救的目标
        int _tonight_poisoned_player;   // 女巫毒的目标
        bool if_antidote_used;          // 女巫解药使用状态
        bool if_poison_used;            // 女巫毒药使用状态
    };

} // namespace WerewolfGame
