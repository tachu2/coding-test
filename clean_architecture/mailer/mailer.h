#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// メール送信を担当するクラス
// 問題点：具象クラスで、インターフェースが定義されていない
class Mailer {
private:
    string smtp_host;
    int smtp_port;
    string from_address;

public:
    Mailer(const string& host, int port, const string& from)
        : smtp_host(host), smtp_port(port), from_address(from) {}
    
    // ユーザー情報更新通知メールを送信（標準出力に出力）
    void send_update_notification(const string& to, const vector<string>& changed_fields) {
        cout << "[EMAIL送信] To: " << to 
             << " | 件名: ユーザー情報が更新されました | 変更項目: ";
        
        for (size_t i = 0; i < changed_fields.size(); i++) {
            cout << changed_fields[i];
            if (i < changed_fields.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
    
    // メールアドレス変更確認メールを送信
    void send_email_change_confirmation(const string& old_email, const string& new_email) {
        cout << "[EMAIL送信] To: " << old_email 
             << " | 件名: メールアドレス変更のお知らせ | 新しいアドレス: " 
             << new_email << endl;
    }
};

