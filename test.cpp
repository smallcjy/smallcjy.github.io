#include<bits/stdc++.h>
#include<limits.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

TreeNode* buildTreeFromLevelOrder(const vector<string>& nodes) {
    if (nodes.empty() || nodes[0] == "null") return nullptr;
    
    TreeNode* root = new TreeNode(stoi(nodes[0]));
    queue<TreeNode*> q;
    q.push(root);
    
    int i = 1;
    while (!q.empty() && i < nodes.size()) {
        TreeNode* curr = q.front();
        q.pop();
        
        if (i < nodes.size() && nodes[i] != "null") {
            curr->left = new TreeNode(stoi(nodes[i]));
            q.push(curr->left);
        }
        i++;
        
        if (i < nodes.size() && nodes[i] != "null") {
            curr->right = new TreeNode(stoi(nodes[i]));
            q.push(curr->right);
        }
        i++;
    }
    
    return root;
}

int res = 0;

int dfs(TreeNode* root) {
    if (!root) return 0;
    if (!root->left && !root->right) return 1;

    int l = dfs(root->left);
    int r = dfs(root->right);

    int lv = (root->left && root->val == root->left->val) ? l : 0;
    int rv = (root->right && root->val == root->right->val) ? r : 0;

    res = max(res, 1 + lv + rv);
    return 1 + max(lv, rv);
}

int main() {
    int n;
    cin >> n;
    vector<string> strs(n);
    for (int i = 0; i < n; i++) cin >> strs[i];
    TreeNode* root = buildTreeFromLevelOrder(strs);
    dfs(root);
    cout << res; // 输出 res 而非 dfs(root) 的返回值
    return 0;
}