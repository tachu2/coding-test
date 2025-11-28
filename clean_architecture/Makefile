# コンパイラとフラグ
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror

# ターゲット実行ファイル名
TARGET = a.out

# ソースファイル
SOURCES = main.cpp

# ヘッダーファイル
HEADERS = model/user.h database/database.h mailer/mailer.h controller/user_controller.h

# デフォルトターゲット
all: $(TARGET)

# 実行ファイルのビルド
$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

# 実行
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

re: clean all

.PHONY: all run clean rebuild

