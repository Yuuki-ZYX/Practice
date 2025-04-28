#include <vector>
#include <string>
#include <iostream>

// 计算模式串的 LPS (Longest Proper Prefix which is also Suffix) 数组
// LPS[i] 表示 pattern[0...i] 的最长相等前后缀的长度
void computeLPSArray(const std::string &pattern, std::vector<int> &lps)
{
    int m = pattern.length();
    lps.assign(m, 0); // lps 数组初始化为 0
    int length = 0;   // 当前最长相等前后缀的长度，初始为 0
    int i = 1;        // 从 pattern[1] 开始计算 lps

    while (i < m)
    {
        // 尝试扩展当前的 'length'
        // 比较 pattern[i] 和 pattern[length]
        if (pattern[i] == pattern[length])
        {
            // 匹配成功！
            // 说明我们找到了一个更长的相等前后缀
            length++;        // 长度增加 1
            lps[i] = length; // 记录 lps[i] 的值
            i++;             // 继续计算下一个 lps 值
        }
        else
        {
            // 不匹配！ pattern[i] != pattern[length]
            // 这意味着以 pattern[i] 结尾的子串，其最长相等前后缀
            // 不能通过简单地在长度为 'length' 的前后缀后面追加字符得到。
            // 我们需要寻找一个更短的候选前后缀长度。

            if (length != 0)
            {
                // 如果 length 不为 0，说明在 pattern[0...i-1] 中存在
                // 一个长度为 'length' 的相等前后缀。
                // 即：pattern[0...length-1] == pattern[i-length...i-1]

                // 现在 pattern[i] 和 pattern[length] 不匹配了。
                // 我们不能直接假设次长的就是 length-1。
                // 我们需要找到 pattern[0...length-1] 这个前缀自身的
                // 最长相等前后缀。为什么？

                // 思考：我们希望找到一个 j < length，使得
                // pattern[0...j-1] == pattern[i-j...i-1]
                // 并且 pattern[j] == pattern[i]

                // KMP 的精髓在于：我们已经知道 pattern[0...length-1]
                // 是 pattern[0...i-1] 的一个前缀，并且它也等于
                // pattern[i-length...i-1] (这是 'length' 的定义)。

                // 因此，pattern[0...length-1] 的最长相等前后缀，
                // 必然也是 pattern[i-length...i-1] 的一个后缀。

                // 而 pattern[0...length-1] 的最长相等前后缀的长度
                // 正好存储在 lps[length - 1] 中！

                // 所以，我们将 length 更新为 lps[length - 1]。
                // 这相当于把候选的相等前后缀长度缩短到下一个可能的值。
                // 注意：我们 *不* 增加 i，因为我们还需要用当前的
                // pattern[i] 去和新的 pattern[length] (即 pattern[lps[length-1]])
                // 进行比较。
                length = lps[length - 1];
            }
            else // length == 0
            {
                // 如果 length 已经是 0，说明 pattern[i] 连 pattern[0] 都
                // 不匹配。或者说，在 pattern[0...i-1] 中根本不存在
                // 任何相等的前后缀。
                // 因此，对于 pattern[0...i]，其最长相等前后缀长度只能是 0。
                lps[i] = 0;
                i++; // 处理下一个字符
            }
        }
    }
}

// KMP 搜索函数
// 返回所有匹配项在文本中的起始索引（0-based）
std::vector<int> KMPSearch(const std::string &text, const std::string &pattern)
{
    int n = text.length();
    int m = pattern.length();
    std::vector<int> results; // 存储匹配结果的索引

    if (m == 0 || n == 0 || m > n)
    {
        return results; // 处理空串或模式串比文本串长的情况
    }

    // 1. 计算 LPS 数组
    std::vector<int> lps(m);
    computeLPSArray(pattern, lps);

    // 2. 执行搜索
    int i = 0; // text 的索引
    int j = 0; // pattern 的索引
    while (i < n)
    {
        if (pattern[j] == text[i])
        {
            i++;
            j++;
        }

        if (j == m)
        {
            // 找到一个完整匹配
            results.push_back(i - m); // 记录匹配的起始位置
            // 继续搜索下一个可能的匹配
            // 利用 lps 数组移动模式串，避免从头开始比较
            j = lps[j - 1];
        }
        else if (i < n && pattern[j] != text[i])
        {
            // 发生失配
            if (j != 0)
            {
                // 利用 lps 数组将模式串向右滑动
                j = lps[j - 1];
            }
            else
            {
                // 如果 j 已经是 0，说明模式串的第一个字符就不匹配
                // 直接移动文本串的指针
                i++;
            }
        }
    }
    return results;
}

// --- 示例用法 ---
int main()
{
    std::string text = "ABABDABACDABABCABAB";
    std::string pattern = "ABABCABAB";

    std::vector<int> matches = KMPSearch(text, pattern);

    if (matches.empty())
    {
        std::cout << "Pattern not found in text." << std::endl;
    }
    else
    {
        std::cout << "Pattern found at indices: ";
        for (size_t k = 0; k < matches.size(); ++k)
        {
            std::cout << matches[k] << (k == matches.size() - 1 ? "" : ", ");
        }
        std::cout << std::endl; // 输出: Pattern found at indices: 10
    }

    std::string text2 = "AAAAABAAABA";
    std::string pattern2 = "AAAA";
    matches = KMPSearch(text2, pattern2);
    if (matches.empty())
    {
        std::cout << "Pattern not found in text." << std::endl;
    }
    else
    {
        std::cout << "Pattern found at indices: ";
        for (size_t k = 0; k < matches.size(); ++k)
        {
            std::cout << matches[k] << (k == matches.size() - 1 ? "" : ", ");
        }
        std::cout << std::endl; // 输出: Pattern found at indices: 0, 1, 6
    }

    return 0;
}