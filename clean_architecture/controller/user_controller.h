#pragma once
#include "../model/user.h"
#include "../database/database.cpp"
#include "../mailer/mailer.cpp"
#include <stdexcept>
#include <cctype>
#include <map>
#include <string>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// ユーザー更新処理を行うコントローラー
class UserController {
private:
    Database* db;
    Mailer* mailer;
    
    // 時間文字列をtime_pointに変換
    system_clock::time_point parse_time(const string& time_str) {
        if (time_str.empty()) {
            return system_clock::now();
        }
        istringstream ss(time_str);
        time_t t;
        ss >> t;
        return system_clock::from_time_t(t);
    }
    
    // time_pointを文字列に変換
    string time_to_string(const system_clock::time_point& tp) {
        time_t t = system_clock::to_time_t(tp);
        ostringstream ss;
        ss << t;
        return ss.str();
    }
    
    // 日数の差を計算
    long days_between(const string& time_str1, const string& time_str2) {
        auto tp1 = parse_time(time_str1);
        auto tp2 = parse_time(time_str2);
        auto diff = duration_cast<hours>(tp2 - tp1);
        return diff.count() / 24;
    }

public:
    UserController(Database* database, Mailer* mail)
        : db(database), mailer(mail) {}
    
    // ユーザー情報を更新する
    map<string, string> update_user(const string& user_id, const UpdateUserRequest& request) {
        // ユーザーを取得
        map<string, string>* user = db->find_user_by_id(user_id);
        if (!user) {
            throw runtime_error("User not found");
        }
        
        string now_str = time_to_string(system_clock::now());
        
        // 30日制限チェック
        if (user->find("updated_at") != user->end()) {
            long days_since_last_change = days_between((*user)["updated_at"], now_str);
            
            if (days_since_last_change < 30) {
                throw runtime_error("User information can only be updated once every 30 days");
            }
        }
        
        bool has_important_change = false;
        bool email_changed = false;
        string old_email;
        
        // 名前の更新
        if (request.name.has_value()) {
            string new_name = request.name.value();
            
            if (new_name.empty()) {
                throw runtime_error("Name cannot be empty");
            }
            
            // 数字のみチェック
            bool all_digits = true;
            for (char c : new_name) {
                if (!isdigit(c)) {
                    all_digits = false;
                    break;
                }
            }
            if (all_digits) {
                throw runtime_error("Name cannot be only numbers");
            }
            
            (*user)["name"] = new_name;
            has_important_change = true;
        }
        
        // メールアドレスの更新
        if (request.email.has_value()) {
            string new_email = request.email.value();
            old_email = (*user)["email"];
            
            if (new_email.find('@') == string::npos) {
                throw runtime_error("Invalid email format");
            }
            
            (*user)["email"] = new_email;
            has_important_change = true;
            email_changed = true;
        }
        
        // 電話番号の更新
        if (request.phone.has_value()) {
            (*user)["phone"] = request.phone.value();
        }
        
        // 更新日時を更新
        (*user)["updated_at"] = now_str;
        db->update_user(*user);
        
        // メールアドレス変更時は確認メールを送信
        if (email_changed) {
            mailer->send_email_change_confirmation(old_email);
        }
        
        // 重要な変更（名前またはメール）があった場合は通知メールを送信
        if (has_important_change) {
            mailer->send_update_notification((*user)["email"]);
        }
        
        return *user;
    }
};

