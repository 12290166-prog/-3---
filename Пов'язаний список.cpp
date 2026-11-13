#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <unordered_set>
#include <limits>
#include <cstdlib>
#include <windows.h>

using namespace std;

struct ListNode {
    int val;
    ListNode* next;

    explicit ListNode(int value, ListNode* nextNode = nullptr)
        : val(value), next(nextNode) {
    }
};

ListNode* createList(const vector<int>& values) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int value : values) {
        tail->next = new ListNode(value);
        tail = tail->next;
    }
    return dummy.next;
}

void printList(const ListNode* head) {
    cout << "[";
    bool first = true;
    while (head != nullptr) {
        if (!first) cout << ", ";
        cout << head->val;
        first = false;
        head = head->next;
    }
    cout << "]";
}

void freeList(ListNode* head) {
    while (head != nullptr) {
        ListNode* nextNode = head->next;
        delete head;
        head = nextNode;
    }
}

bool parseInteger(const string& text, long long& result) {
    istringstream stream(text);
    if (!(stream >> result)) return false;
    stream >> ws;
    return stream.eof();
}

string readLine() {
    string line;
    if (!getline(cin, line)) {
        cout << "\nВведення завершено. Програму закрито.\n";
        exit(0);
    }
    return line;
}

int readInt(const string& prompt, int minimum, int maximum) {
    while (true) {
        cout << prompt;
        long long value;
        if (parseInteger(readLine(), value) &&
            value >= minimum && value <= maximum) {
            return static_cast<int>(value);
        }
        cout << "Помилка! Введіть ціле число від " << minimum
            << " до " << maximum << ".\n";
    }
}

vector<int> readValues(int count, int minimum, int maximum,
    bool sorted = false, bool unique = false,
    bool noLeadingZero = false) {
    if (count == 0) return {};

    while (true) {
        cout << "Введіть елементи списку (кількість: " << count
            << ", значення від " << minimum << " до " << maximum
            << ") через пробіл:\n> ";
        istringstream stream(readLine());
        vector<int> values;
        string token;
        bool valid = true;
        unordered_set<int> seen;

        while (stream >> token) {
            long long number;
            if (!parseInteger(token, number) || number < minimum ||
                number > maximum || values.size() >= static_cast<size_t>(count)) {
                valid = false;
                break;
            }
            int value = static_cast<int>(number);
            if (sorted && !values.empty() && value < values.back()) {
                valid = false;
                break;
            }
            if (unique && !seen.insert(value).second) {
                valid = false;
                break;
            }
            values.push_back(value);
        }

        if (values.size() != static_cast<size_t>(count)) valid = false;
        if (valid && noLeadingZero && count > 1 && values[0] == 0) {
            valid = false;
        }

        if (valid) return values;
        cout << "Некоректні дані. Перевірте кількість і діапазон значень";
        if (sorted) cout << ", порядок неспадання";
        if (unique) cout << ", унікальність";
        if (noLeadingZero) cout << ", відсутність початкових нулів";
        cout << ". Спробуйте ще раз.\n";
    }
}

vector<int> readListValues(const string& title, int minimumCount,
    int maximumCount, int minimumValue, int maximumValue,
    bool sorted = false, bool unique = false,
    bool noLeadingZero = false) {
    cout << "\n" << title << "\n";
    int count = readInt("Кількість вузлів: ", minimumCount, maximumCount);
    return readValues(count, minimumValue, maximumValue,
        sorted, unique, noLeadingZero);
}

ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
    ListNode dummy(0);
    ListNode* tail = &dummy;

    while (list1 != nullptr && list2 != nullptr) {
        if (list1->val <= list2->val) {
            tail->next = list1;
            list1 = list1->next;
        }
        else {
            tail->next = list2;
            list2 = list2->next;
        }
        tail = tail->next;
    }
    tail->next = (list1 != nullptr) ? list1 : list2;
    return dummy.next;
}

ListNode* deleteDuplicates(ListNode* head) {
    ListNode* current = head;
    while (current != nullptr && current->next != nullptr) {
        if (current->val == current->next->val) {
            ListNode* duplicate = current->next;
            current->next = duplicate->next;
            delete duplicate;
        }
        else {
            current = current->next;
        }
    }
    return head;
}

bool hasCycle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}

void reorderList(ListNode* head) {
    if (head == nullptr || head->next == nullptr) return;

    ListNode* slow = head;
    ListNode* fast = head;
    while (fast->next != nullptr && fast->next->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }

    ListNode* second = slow->next;
    slow->next = nullptr;
    ListNode* previous = nullptr;
    while (second != nullptr) {
        ListNode* nextNode = second->next;
        second->next = previous;
        previous = second;
        second = nextNode;
    }
    second = previous;

    ListNode* first = head;
    while (second != nullptr) {
        ListNode* nextFirst = first->next;
        ListNode* nextSecond = second->next;
        first->next = second;
        second->next = nextFirst;
        first = nextFirst;
        second = nextSecond;
    }
}

void deleteNode(ListNode* node) {
    ListNode* successor = node->next;
    node->val = successor->val;
    node->next = successor->next;
    delete successor;
}

ListNode* doubleIt(ListNode* head) {
    if (head == nullptr) return nullptr;
    if (head->val >= 5) head = new ListNode(0, head);

    for (ListNode* current = head; current != nullptr; current = current->next) {
        int carryFromNext = (current->next != nullptr &&
            current->next->val >= 5) ? 1 : 0;
        current->val = (current->val * 2) % 10 + carryFromNext;
    }
    return head;
}

struct SmallerNodeFirst {
    bool operator()(const ListNode* a, const ListNode* b) const {
        return a->val > b->val;
    }
};

ListNode* mergeKLists(const vector<ListNode*>& lists) {
    priority_queue<ListNode*, vector<ListNode*>, SmallerNodeFirst> heap;
    for (ListNode* head : lists) {
        if (head != nullptr) heap.push(head);
    }

    ListNode dummy(0);
    ListNode* tail = &dummy;
    while (!heap.empty()) {
        ListNode* smallest = heap.top();
        heap.pop();
        if (smallest->next != nullptr) heap.push(smallest->next);
        tail->next = smallest;
        tail = smallest;
    }
    tail->next = nullptr;
    return dummy.next;
}

ListNode* reverseKGroup(ListNode* head, int k) {
    if (k == 1) return head;
    ListNode dummy(0, head);
    ListNode* groupPrevious = &dummy;

    while (true) {
        ListNode* kth = groupPrevious;
        for (int i = 0; i < k && kth != nullptr; ++i) {
            kth = kth->next;
        }
        if (kth == nullptr) break;

        ListNode* groupNext = kth->next;
        ListNode* previous = groupNext;
        ListNode* current = groupPrevious->next;
        while (current != groupNext) {
            ListNode* nextNode = current->next;
            current->next = previous;
            previous = current;
            current = nextNode;
        }

        ListNode* previousStart = groupPrevious->next;
        groupPrevious->next = kth;
        groupPrevious = previousStart;
    }
    return dummy.next;
}

ListNode* partitionList(ListNode* head, int x) {
    ListNode lessDummy(0);
    ListNode greaterDummy(0);
    ListNode* lessTail = &lessDummy;
    ListNode* greaterTail = &greaterDummy;

    while (head != nullptr) {
        ListNode* nextNode = head->next;
        head->next = nullptr;
        if (head->val < x) {
            lessTail->next = head;
            lessTail = head;
        }
        else {
            greaterTail->next = head;
            greaterTail = head;
        }
        head = nextNode;
    }
    lessTail->next = greaterDummy.next;
    return lessDummy.next;
}

void task1() {
    vector<int> a = readListValues("Перший відсортований список:", 0, 50,
        -100, 100, true);
    vector<int> b = readListValues("Другий відсортований список:", 0, 50,
        -100, 100, true);
    ListNode* head1 = createList(a);
    ListNode* head2 = createList(b);
    cout << "Перший список: "; printList(head1);
    cout << "\nДругий список: "; printList(head2);
    ListNode* result = mergeTwoLists(head1, head2);
    cout << "\nОб'єднаний список: "; printList(result);
    cout << "\n";
    freeList(result);
}

void task2() {
    vector<int> values = readListValues("Відсортований список:", 0, 300,
        -100, 100, true);
    ListNode* head = createList(values);
    cout << "Початковий список: "; printList(head);
    head = deleteDuplicates(head);
    cout << "\nСписок без дублікатів: "; printList(head);
    cout << "\n";
    freeList(head);
}

void task3() {
    vector<int> values = readListValues("Список для перевірки циклу:", 0,
        10000, -100000, 100000);
    int length = static_cast<int>(values.size());
    int pos = readInt("Індекс вузла, куди веде хвіст (-1 — без циклу): ",
        -1, length - 1);
    ListNode* head = createList(values);
    cout << "Початковий список: "; printList(head);

    ListNode* tail = nullptr;
    ListNode* cycleTarget = nullptr;
    int index = 0;
    for (ListNode* current = head; current != nullptr; current = current->next) {
        if (index == pos) cycleTarget = current;
        tail = current;
        ++index;
    }
    if (tail != nullptr && pos != -1) tail->next = cycleTarget;

    bool result = hasCycle(head);
    cout << "\nЧи є цикл? " << (result ? "true (так)" : "false (ні)") << "\n";
    if (tail != nullptr && pos != -1) tail->next = nullptr;
    freeList(head);
}

void task4() {
    vector<int> values = readListValues("Список для перевпорядкування:", 1,
        50000, 1, 1000);
    ListNode* head = createList(values);
    cout << "Початковий список: "; printList(head);
    reorderList(head);
    cout << "\nПеревпорядкований список: "; printList(head);
    cout << "\n";
    freeList(head);
}

void task5() {
    vector<int> values = readListValues("Список з унікальними значеннями:", 2,
        1000, -1000, 1000, false, true);
    ListNode* head = createList(values);
    cout << "Початковий список: "; printList(head);
    cout << "\n";

    ListNode* target = nullptr;
    while (target == nullptr) {
        int value = readInt("Значення вузла для видалення (не останнього): ",
            -1000, 1000);
        for (ListNode* current = head; current != nullptr; current = current->next) {
            if (current->val == value && current->next != nullptr) {
                target = current;
                break;
            }
        }
        if (target == nullptr) cout << "Вузол не знайдено або він останній. Повторіть.\n";
    }

    deleteNode(target); 
    cout << "Список після видалення: "; printList(head);
    cout << "\n";
    freeList(head);
}

void task6() {
    vector<int> values = readListValues("Цифри числа (від старшої до молодшої):",
        1, 10000, 0, 9, false, false, true);
    ListNode* head = createList(values);
    cout << "Початкові цифри: "; printList(head);
    head = doubleIt(head);
    cout << "\nПодвоєне число: "; printList(head);
    cout << "\n";
    freeList(head);
}

void task7() {
    int countLists = readInt("Кількість відсортованих списків k (0..10000): ",
        0, 10000);
    vector<ListNode*> heads;
    heads.reserve(countLists);
    int totalNodes = 0;

    for (int i = 0; i < countLists; ++i) {
        cout << "\nСписок №" << i + 1 << ":\n";
        int limit = min(500, 10000 - totalNodes);
        int count = readInt("Кількість вузлів (0.." + to_string(limit) + "): ",
            0, limit);
        vector<int> values = readValues(count, -10000, 10000, true);
        heads.push_back(createList(values));
        totalNodes += count;
    }

    ListNode* result = mergeKLists(heads);
    cout << "\nОб'єднаний відсортований список: "; printList(result);
    cout << "\n";
    freeList(result);
}

void task8() {
    vector<int> values = readListValues("Список для розвороту груп:", 1,
        5000, 0, 1000);
    int k = readInt("Розмір групи k: ", 1, static_cast<int>(values.size()));
    ListNode* head = createList(values);
    cout << "Початковий список: "; printList(head);
    head = reverseKGroup(head, k);
    cout << "\nСписок після розвороту груп: "; printList(head);
    cout << "\n";
    freeList(head);
}

void task9() {
    vector<int> values = readListValues("Список для розділення:", 0,
        200, -100, 100);
    int x = readInt("Введіть число x (-200..200): ", -200, 200);
    ListNode* head = createList(values);
    cout << "Початковий список: "; printList(head);
    head = partitionList(head, x);
    cout << "\nРозділений список: "; printList(head);
    cout << "\n";
    freeList(head);
}

#ifndef LINKED_LIST_TEST
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << "Аналітичне завдання 3 - Пов'язаний список\n";

    while (true) {
        cout << "1. Об'єднати два відсортовані списки\n"
            << "2. Видалити дублікати з відсортованого списку\n"
            << "3. Перевірити наявність циклу в списку\n"
            << "4. Перевпорядкувати список\n"
            << "5. Видалити вузол без доступу до голови списку\n"
            << "6. Подвоїти число, задане списком цифр\n"
            << "7. Об'єднати k відсортованих списків\n"
            << "8. Розвернути вузли групами по k\n"
            << "9. Розділити список за значенням x\n"
            << "0. Завершити роботу\n";
        int choice = readInt("Оберіть завдання (0..9): ", 0, 9);
        if (choice == 0) {
            cout << "Роботу завершено.\n";
            return 0;
        }
        cout << "\n";
        switch (choice) {
        case 1: task1(); break;
        case 2: task2(); break;
        case 3: task3(); break;
        case 4: task4(); break;
        case 5: task5(); break;
        case 6: task6(); break;
        case 7: task7(); break;
        case 8: task8(); break;
        case 9: task9(); break;
        }
    }
}
#endif
