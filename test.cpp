#include <iostream>
#include <vector>
using namespace std;

struct Node {
    int val; Node* next;
    Node(int val_) : val(val_), next(nullptr) {}
};

Node* build(vector<int>& nums) {
	Node* dummy = new Node(0);
   	Node* cur = dummy;
    for(int n : nums) {
        cur->next = new Node(n);
        cur = cur->next;
    }
    
    return dummy->next;
}

Node* merge(Node* a, Node* b) {
    Node* dummy = new Node(0);
    Node* cur = dummy;
    
    while(a && b) {
        if (a->val <= b->val) {
            cur->next = a;
            a = a->next;
        } else {
            cur->next = b;
            b = b->next;
        }
       	cur = cur->next;
    }
    
    if (a) cur->next =a;
    else if (b) cur->next = b;
    return dummy->next;
}

Node* sort(Node* head) {
    if (!head || !head->next) return head;
    Node* dummy = new Node(0); dummy->next = head;
    
    Node* fast = head;
    Node* slow = dummy;
    
    while(fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    Node* right = slow->next;
    slow->next = nullptr;
    Node* left = head;
    
    Node* sortl = sort(left);
    Node* sortr = sort(right);
    
    return merge(sortl, sortr);
}

int main() {
    int n;
    cin >> n;
    vector<int> nums(n);
    for(int i = 0; i < n; i++) cin>>nums[i];
    Node* head = build(nums);
    Node* ans = sort(head);
    
    while(ans) {
        cout << ans->val << ' ';
        ans = ans->next;
    }
    
    return 0;
}
// 64 位输出请用 printf("%lld")