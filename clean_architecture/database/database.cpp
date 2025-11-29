#pragma once
#include <map>
#include <string>
#include <stdexcept>

using namespace std;

// データベース
// Warning: 本当のDBとして扱うこと
// これらのメソッドを呼ぶとDB上にデータが永続化されるもとのして扱うこと
class Database {
private:
    map<string, map<string, string>> users;
    int next_id = 1;

public:
    Database() {}
    
    // ユーザーを作成（string, stringのmapを受け取る）
    map<string, string> create_user(const map<string, string>& user_data) {
        // IDが指定されていない場合は自動生成
        string id;
        if (user_data.find("id") != user_data.end()) {
            id = user_data.at("id");
        } else {
            id = to_string(next_id++);
        }
        
        map<string, string> user_with_id = user_data;
        user_with_id["id"] = id;
        users[id] = user_with_id;
        return user_with_id;
    }
    
    // ユーザーを取得
    map<string, string>* find_user_by_id(const string& id) {
        auto it = users.find(id);
        if (it != users.end()) {
            return &it->second;
        }
        return nullptr;
    }
    
    // ユーザー情報を更新
    void update_user(const map<string, string>& user_data) {
        if (user_data.find("id") == user_data.end()) {
            throw runtime_error("User ID is required for update");
        }
        
        string id = user_data.at("id");
        auto it = users.find(id);
        if (it != users.end()) {
            it->second = user_data;
        } else {
            throw runtime_error("User not found");
        }
    }
};

