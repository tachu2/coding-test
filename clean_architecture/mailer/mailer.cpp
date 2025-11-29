#pragma once
#include <iostream>
#include <string>

using namespace std;

// メール送信サービス
// Warning: このクラスのメソッドを呼ぶと本当にユーサーにメールが送信されるので外部APIとして扱うこと
class Mailer {
public:
    Mailer() {}
    
    // ユーザー情報更新通知メールを送信
    void send_update_notification(const string& to) {
        cout << "[EMAIL送信] To: " << to 
             << " | 件名: ユーザー情報が更新されました" << endl;
    }
    
    // メールアドレス変更確認メールを送信
    void send_email_change_confirmation(const string& to) {
        cout << "[EMAIL送信] To: " << to 
             << " | 件名: メールアドレス変更のお知らせ" << endl;
    }
};

