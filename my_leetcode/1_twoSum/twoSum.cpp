class Solution {
    public:
        vector<int> twoSum(vector<int> &numbers, int target) {
            unordered_map<int, int> m;
            vector<int> result;
            for(int i = 0; i < numbers.size(); ++i){
                /*one:number[i]
                  two:target-numbers[i]
                  two会与one匹配,所以把one的索引[i]作为two的value
                 */
                if(m.find(numbers[i]) == m.end()){//生成map
                    m[target-numbers[i]] = i;
                }else{
                    result.push_back(m[numbers[i]] + 1);
                    result.push_back(i+1);
                    break;
                }
            }
            return result;   
        }

};
