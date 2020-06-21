
#include <iostream>
#include <string>
using namespace std;

  struct TreeNode {
      int val;
      TreeNode *left;
      TreeNode *right;
      TreeNode(int x) : val(x), left(NULL), right(NULL) {}
  };

class Solution {
public:
    int maxPathSum(TreeNode* root) {
        finres = root->val;
        int left = maxSum(root->left);
        int right = maxSum(root->right);
        int rootres = root->val;
        if (left > 0) rootres += left;
        if (right > 0) rootres += right;

        return max(finres, rootres);
    }

    int maxSum(TreeNode *root) {
        if (root == nullptr) return 0;
        int res = root->val;
        int left = maxSum(root->left);
        int right = maxSum(root->right);

        std::cout << root->val << ": " << left << " " << right << std::endl;
        res = max(res, res + left + right);
        res = max(res, res + left);
        res = max(res, res + right);
        res = max(0, res);
        if (res > finres) finres = res;
        return res;
    }

private:
    int finres;
};


int main() {
    Solution so;
    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(-2);
    root->right = new TreeNode(3);
    std::cout << so.maxPathSum(root);
}
