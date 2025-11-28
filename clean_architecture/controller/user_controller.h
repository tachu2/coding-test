#pragma once
#include "../model/user.h"
#include "../database/database.h"
#include "../mailer/mailer.h"
#include <stdexcept>
#include <cctype>

using namespace std;
using namespace std::chrono;

// ユーザー更新処理を行うコントローラー
class UserController {
private:
    Database* db;
    Mailer* mailer;

public:
    UserController(Database* database, Mailer* mail)
        : db(database), mailer(mail) {}
    
    // ユーザー情報を更新する
    User update_user(int user_id, const UpdateUserRequest& request) {
        // ユーザーを取得
        User* user = db->find_user_by_id(user_id);
        if (!user) {
            throw runtime_error("User not found");
        }
        
        bool has_important_change = false;
        
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
            
            auto days_since_last_change = duration_cast<hours>(
                system_clock::now() - user->last_name_update
            ).count() / 24;
            
            if (days_since_last_change < 90) {
                throw runtime_error("Name can only be changed once every 90 days");
            }
            
            user->name = new_name;
            user->last_name_update = system_clock::now();
            has_important_change = true;
        }
        
        // メールアドレスの更新
        if (request.email.has_value()) {
            string new_email = request.email.value();
            string old_email = user->email;
            
            if (new_email.find('@') == string::npos) {
                throw runtime_error("Invalid email format");
            }
            
            auto days_since_last_change = duration_cast<hours>(
                system_clock::now() - user->last_email_update
            ).count() / 24;
            
            if (days_since_last_change < 30) {
                throw runtime_error("Email can only be changed once every 30 days");
            }
            
            user->email = new_email;
            user->last_email_update = system_clock::now();
            has_important_change = true;
            
            mailer->send_email_change_confirmation(old_email);
        }
        
        // 電話番号の更新
        if (request.phone.has_value()) {
            user->phone = request.phone.value();
            user->last_phone_update = system_clock::now();
        }
        
        db->update_user(*user);
        
        if (has_important_change) {
            mailer->send_update_notification(user->email);
        }
        
        return *user;
    }
};

