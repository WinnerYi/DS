//11327217 蔡易勳     11327222 林暐倫 
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <climits>
void PrintTitle();
void SkipSpace(std::string &str);
std::string ReadInput();

struct Node {
  int hp;                    // 生命值（BST key）
  std::vector<int> ids;      // 相同 hp 的所有編號
  Node* left;
  Node* right;

  Node(int hp_, int id)
      : hp(hp_), left(nullptr), right(nullptr) {
      ids.push_back(id);
  } 
};

class BinarySearchTreeIterative {
 private:
    Node* root;

 public:
    BinarySearchTreeIterative() : root(nullptr) {}

    // 插入：生命值 + 編號
    void insert(int hp, int id) {
        if (root == nullptr) {
            root = new Node(hp, id);
            return;
        }

        Node* current = root;
        Node* parent = nullptr;

        while (current != nullptr) {
            parent = current;

            if (hp < current->hp) {
                current = current->left;
            }
            else if (hp > current->hp) {
                current = current->right;
            }
            else {
                // hp 相同 → 存到同一個節點
                current->ids.push_back(id);
                return;
            }
        }

        if (hp < parent->hp)
            parent->left = new Node(hp, id);
        else
            parent->right = new Node(hp, id);
    }

    // 搜尋：回傳該生命值的所有編號
    std::vector<int>* search(int hp) {
        Node* current = root;
        while (current != nullptr) {
            if (current->hp == hp)
                return &current->ids;
            else if (hp < current->hp)
                current = current->left;
            else
                current = current->right;
        }
        return nullptr;
    }

    // Inorder（由小到大生命值）
    void inorder() {
        std::vector<Node*> stack;
        Node* current = root;

        while (current != nullptr || !stack.empty()) {
            while (current != nullptr) {
                stack.push_back(current);
                current = current->left;
            }

            current = stack.back();
            stack.pop_back();

            std::cout << "HP: " << current->hp << " IDs: ";
            for (int id : current->ids)
                std::cout << id << " ";
            std::cout << "\n";

            current = current->right;
        }
    }

    // 刪除整個生命值節點（含所有 id）
    void deleteNode(int hp) {
        Node* parent = nullptr;
        Node* current = root;
        bool isLeftChild = false;

        while (current != nullptr && current->hp != hp) {
            parent = current;
            if (hp < current->hp) {
                isLeftChild = true;
                current = current->left;
            } else {
                isLeftChild = false;
                current = current->right;
            }
        }

        if (current == nullptr) return;

        // case 1 / 2：0 or 1 child
        if (current->left == nullptr || current->right == nullptr) {
            Node* child = (current->left) ? current->left : current->right;

            if (parent == nullptr)
                root = child;
            else if (isLeftChild)
                parent->left = child;
            else
                parent->right = child;

            delete current;
        }
        // case 3：2 children
        else {
            Node* succParent = current;
            Node* succ = current->right;

            while (succ->left != nullptr) {
                succParent = succ;
                succ = succ->left;
            }

            current->hp = succ->hp;
            current->ids = succ->ids;

            if (succParent->left == succ)
                succParent->left = succ->right;
            else
                succParent->right = succ->right;

            delete succ;
        }
    }

    int height() {
        if (root == nullptr) return 0;  // 空樹

        std::queue<Node*> q;
        q.push(root);
        int h = 0;

        while (!q.empty()) {
            int levelSize = q.size();
            h++;  // 每跑一次 while = 一層 = 高度 +1

            for (int i = 0; i < levelSize; i++) {
                Node* cur = q.front();
                q.pop();

                if (cur->left)
                    q.push(cur->left);
                if (cur->right)
                    q.push(cur->right);
            }
        }
        return h;
    }

    void clear() {
      deleteTreeRecursive(root);
      root = nullptr;
    }

    ~BinarySearchTreeIterative() {
      clear();
    }

private:
    void deleteTreeRecursive(Node* node) {
        if (!node) return;
        deleteTreeRecursive(node->left);
        deleteTreeRecursive(node->right);
        delete node;
    }
};

class Raichu{  // 雷丘放電
 private:
  int id;
  std::string name;
  std::string type1;
  std::string type2;
  int total;
  int hp;
  int attack;
  int defense;
  int sp_atk;
  int sp_def;
  int speed;
  int generation;
  std::string legendary;

  public:
    void setRaichu(int id, std::string name, std::string type1, std::string type2,
                int total, int hp, int attack, int defense,
                int sp_atk, int sp_def, int speed,
                int generation, std::string legendary) {

      this->id = id;
      this->name = name;
      this->type1 = type1;
      this->type2 = type2;
      this->total = total;
      this->hp = hp;
      this->attack = attack;
      this->defense = defense;
      this->sp_atk = sp_atk;
      this->sp_def = sp_def;
      this->speed = speed;
      this->generation = generation;
      this->legendary = legendary;
    }
    int getid() {
      return id;
    }

    std::string getName() {
      return name;
    }

    std::string getType1() {
      return type1;
    }

    int getHp() {
      return hp;
    }

};

class Pokemon {
 private:
  BinarySearchTreeIterative bst;
  std::vector<Raichu> raichus;
  
 public:
  Pokemon() : bst(), raichus() {}
  void reSet() {
    bst.clear();
    raichus.clear();
    raichus.shrink_to_fit();  // 釋放多餘容量

  }
  bool fetchFile() {
    std::ifstream in;
    std:: cout << "Input a file number [0: quit]: ";
    std::string file_num = ReadInput();
    if (file_num == "0") return false;
    std::string txt_path = "input" + file_num + ".txt";
    in.open(txt_path);
    if(in.fail()){ 
      std::cout << std::endl << "### " << txt_path + " does not exist! ###" << std::endl;
      return false; 
    }
    std::string title;
    std::getline(in, title);

    std::string line;
  while (std::getline(in, line)) {
    std::stringstream ss(line);
    int id, total, hp, attack, defense, sp_atk, sp_def, speed, generation;
    std::string name, type1, type2, legendary;

    std::string id_str, total_str, hp_str, attack_str,
                defense_str, sp_atk_str, sp_def_str,
                speed_str, generation_str;
    std::getline(ss, id_str, '\t');
    std::getline(ss, name, '\t');
    std::getline(ss, type1, '\t');
    std::getline(ss, type2, '\t');
    std::getline(ss, total_str, '\t');
    std::getline(ss, hp_str, '\t');
    std::getline(ss, attack_str, '\t');
    std::getline(ss, defense_str, '\t');
    std::getline(ss, sp_atk_str, '\t');
    std::getline(ss, sp_def_str, '\t');
    std::getline(ss, speed_str, '\t');
    std::getline(ss, generation_str, '\t');
    std::getline(ss, legendary); // 最後一欄到行尾
    id        = std::stoi(id_str);
    total      = std::stoi(total_str);
    hp         = std::stoi(hp_str);
    attack     = std::stoi(attack_str);
    defense    = std::stoi(defense_str);
    sp_atk     = std::stoi(sp_atk_str);
    sp_def     = std::stoi(sp_def_str);
    speed      = std::stoi(speed_str);
    generation = std::stoi(generation_str);

    Raichu r;
    r.setRaichu(id, name, type1, type2,
                total, hp, attack, defense,
                sp_atk, sp_def, speed,
                generation, legendary);
    bst.insert(hp, id);
    raichus.push_back(r);
}   
    in.close();
    return true;
}

int getRaichuSize() {
  return raichus.size();
}

void taskOne() {
  std::cout << "\t#\tName\tType 1\tHP\n";
  for (int i = 0; i < raichus.size(); i++) {
    if (i + 1 < 10) std::cout << "[  " << i+1 << "]";
    else if (i + 1 >= 10) std::cout << "[ " << i+1 << "]";
    else if (i + 1 >= 100) std::cout << "[" << i+1 << "]";
    std::cout << "\t" << raichus[i].getid();
    std::cout << "\t" << raichus[i].getName();
    std::cout << "\t" << raichus[i].getType1();
    std::cout << "\t" << raichus[i].getHp();
    std::cout << "\n";
  }
  std::cout << "HP tree height = " << bst.height() << std::endl;
}


};







int main() {
 
  Pokemon pokemon;
  while (true) {
    PrintTitle();
    int cmd;
    std::cin >> cmd;
    if (std::cin.fail()) { // 檢查輸入是否失敗
        std::cin.clear();              
        std::cin.ignore(10000, '\n');  
        return 0;
    } else if (cmd == 0 ){
      return 0;
    } else if (cmd == 1) {
      pokemon.reSet();
      printf("\n");
      if (pokemon.fetchFile()) {
        pokemon.taskOne();
      }
      
   
      
    } else if (cmd == 2) {
      if (pokemon.getRaichuSize() == 0) {
        std::cout << "\n----- Execute Mission 1 first! -----\n\n";
        continue;
      }
   
 
  
    } else if (cmd == 3) {
      if (pokemon.getRaichuSize() == 0) {
        std::cout << "\n----- Execute Mission 1 first! -----\n\n";
        continue;
      }
    
     
    } else if (cmd == 4) {
      if (pokemon.getRaichuSize() == 0) {
        std::cout << "\n----- Execute Mission 1 first! -----\n\n";
        continue;
      }
    
    } else {
      printf("\n");
      std::cout << "Command does not exist!\n";
    }
    printf("\n");  
  }
}

std::string ReadInput() {
  std::string input;
  while (1) {
    std::getline(std::cin, input);
    SkipSpace(input);
    if (input.empty()) continue;
    else break;
  }
  return input;
}

void SkipSpace(std::string &str) {
  for (int i = 0; i < str.size(); i++) {
    if (str[i] != ' ') break;
    if (str[i] == ' ') {
      str.erase(str.begin() + i);
      i--;
    }
  }
  for (int i = str.size()-1; i >= 0; i--) {
    if (str[i] != ' ') break;
    if (str[i] == ' ') {
      str.erase(str.begin() + i);
    }
  }
}

void PrintTitle () {
  std::cout << "*** (^_^) Data Structure (^o^) ***\n";
  std::cout << "** Binary Search Tree on Pokemon *\n";
  std::cout << "* 0. QUIT                        *\n";
  std::cout << "* 1. Read a file to build HP BST *\n";
  std::cout << "* 2. Range search on HP field    *\n";
  std::cout << "* 3. Delete the min on HP field  *\n";    
  std::cout << "* 4. Rebuild the balanced HP BST *\n";
  std::cout << "**********************************\n";
  std::cout << "Input a choice(0, 1, 2, 3, 4): ";
} 
