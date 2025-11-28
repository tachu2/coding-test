#include <iostream>
#include "database/database.h"
#include "mailer/mailer.h"
#include "controller/user_controller.h"

using namespace std;

int main() {
    cout << "=== ユーザー情報更新システム ===" << endl << endl;
    
    // 初期化
    Database db;
    Mailer mailer("smtp.example.com", 587, "noreply@example.com");
    UserController controller(&db, &mailer);
    
    // テストユーザー作成
    User user = db.create_user("山田太郎", "yamada@example.com", "090-1234-5678");
    cout << "ユーザー作成: ID=" << user.id << ", " << user.name << ", " << user.email << endl << endl;
    
    // 電話番号更新
    cout << "1. 電話番号更新" << endl;
    try {
        UpdateUserRequest req;
        req.phone = "090-9999-8888";
        controller.update_user(user.id, req);
        cout << "✓ 更新成功" << endl;
    } catch (const exception& e) {
        cout << "✗ エラー: " << e.what() << endl;
    }
    cout << endl;
    
    // 名前更新
    cout << "2. 名前更新" << endl;
    try {
        UpdateUserRequest req;
        req.name = "山田花子";
        controller.update_user(user.id, req);
        cout << "✓ 更新成功" << endl;
    } catch (const exception& e) {
        cout << "✗ エラー: " << e.what() << endl;
    }
    cout << endl;
    
    // メールアドレス更新
    cout << "3. メールアドレス更新" << endl;
    try {
        UpdateUserRequest req;
        req.email = "hanako@example.com";
        controller.update_user(user.id, req);
        cout << "✓ 更新成功" << endl;
    } catch (const exception& e) {
        cout << "✗ エラー: " << e.what() << endl;
    }
    cout << endl;
    
    // バリデーションエラー（空の名前）
    cout << "4. バリデーションエラー（空の名前）" << endl;
    try {
        UpdateUserRequest req;
        req.name = "";
        controller.update_user(user.id, req);
        cout << "✓ 更新成功" << endl;
    } catch (const exception& e) {
        cout << "✗ エラー: " << e.what() << endl;
    }
    cout << endl;
    
    // ビジネスルール違反（名前の再変更）
    cout << "5. ビジネスルール違反（名前の再変更）" << endl;
    try {
        UpdateUserRequest req;
        req.name = "田中一郎";
        controller.update_user(user.id, req);
        cout << "✓ 更新成功" << endl;
    } catch (const exception& e) {
        cout << "✗ エラー: " << e.what() << endl;
    }
    
    return 0;
}
