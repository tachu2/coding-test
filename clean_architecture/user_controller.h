#pragma once
#include "user.h"
#include "database.h"
#include "mailer.h"
#include <vector>
#include <stdexcept>
#include <cctype>

using namespace std::chrono;

// ユーザー更新処理を行うコントローラー
// 問題点：全てのロジックがここに詰め込まれている
class UserController {
private:
    Database* db;      // 問題点：具象クラスに依存
    Mailer* mailer;    // 問題点：具象クラスに依存

public:
    UserController(Database* database, Mailer* mail)
        : db(database), mailer(mail) {}
    
    // ユーザー情報を更新する
    // 問題点：バリデーション、ビジネスルール、DB保存、メール送信が全てここに書かれている
    User update_user(int user_id, const UpdateUserRequest& request) {
        // ユーザーを取得
        User* user = db->find_user_by_id(user_id);
        if (!user) {
            throw runtime_error("User not found");
        }
        
        vector<string> changed_fields;
        bool has_important_change = false;
        
        // 名前の更新
        if (request.name.has_value()) {
            string new_name = request.name.value();
            
            // バリデーション（コントローラーで実施）
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
            
            // ビジネスルール：名前変更は90日に1回まで（コントローラーで実施）
            auto days_since_last_change = duration_cast<hours>(
                system_clock::now() - user->last_name_update
            ).count() / 24;
            
            if (days_since_last_change < 90) {
                throw runtime_error("Name can only be changed once every 90 days");
            }
            
            user->name = new_name;
            user->last_name_update = system_clock::now();
            changed_fields.push_back("名前");
            has_important_change = true;
        }
        
        // メールアドレスの更新
        if (request.email.has_value()) {
            string new_email = request.email.value();
            string old_email = user->email;
            
            // バリデーション（コントローラーで実施）
            if (new_email.find('@') == string::npos) {
                throw runtime_error("Invalid email format");
            }
            
            // ビジネスルール：メールアドレス変更は30日に1回まで（コントローラーで実施）
            auto days_since_last_change = duration_cast<hours>(
                system_clock::now() - user->last_email_update
            ).count() / 24;
            
            if (days_since_last_change < 30) {
                throw runtime_error("Email can only be changed once every 30 days");
            }
            
            user->email = new_email;
            user->last_email_update = system_clock::now();
            changed_fields.push_back("メールアドレス");
            has_important_change = true;
            
            // 旧メールアドレスに確認メール送信（コントローラーから直接呼び出し）
            mailer->send_email_change_confirmation(old_email, new_email);
        }
        
        // 電話番号の更新
        if (request.phone.has_value()) {
            user->phone = request.phone.value();
            user->last_phone_update = system_clock::now();
            changed_fields.push_back("電話番号");
        }
        
        // DB保存（コントローラーから直接呼び出し）
        db->update_user(*user);
        
        // 更新通知メール送信（コントローラーで判断して送信）
        // 問題点：通知の判断ロジックがコントローラーにある
        if (has_important_change) {
            mailer->send_update_notification(user->email, changed_fields);
        }
        
        return *user;
    }
};
