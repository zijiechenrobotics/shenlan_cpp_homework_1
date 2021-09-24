#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <chrono>
#include <ctime>

// 用来保存试题 用于给出答错的题目与其正确答案
struct QuestionStr
{
    int kind_of_operation; // 运算类型
    int a; // 第一个数
    int b; // 第二个数
    int user_answer; // 用户答案
    int right_answer; // 正确答案
    int num;
};

std::vector<QuestionStr> wrong_answer_queue; // 存储错误答案

// 计算用户答案用时
int Enter(double& total_time, double& min_time){
    int c;

    auto start = std::chrono::system_clock::now();
    std::cin >> c;
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> sec = end - start;
    double time = sec.count(); //秒
    // std::cout<<time<<std::endl;

    total_time += time;

    // 筛选最短时间
    if(time<min_time)
        min_time = time;

    return c;
}

// 保存错误答案
void SaveAnswer(int count, int kind_of_test, 
    int a, int b, int c, int right_answer){

    QuestionStr question_temp;
    question_temp.num = count; // 题号
    question_temp.kind_of_operation = kind_of_test; // 算法类型:加减乘除
    question_temp.a = a; // 第一个数
    question_temp.b = b; // 第二个数
    question_temp.user_answer = c; // 用户答案
    question_temp.right_answer = right_answer; // 正确答案
    wrong_answer_queue.push_back( question_temp);
}


int main(){

    int num_of_test{0};
    int num_of_right{0}, num_of_wrong{0};
    int a,b,c,kind_of_test,right_answer;
    double total_time{0.0}, min_time{999.9};
    int level, range;

    srand (time(NULL));
    
    std::cout<<"作者: CHENXXX"<<std::endl;
    std::cout<<"欢迎使用算法练习小程序 ^_^"<<std::endl;

    std::cout<<"请输入您要练习的数目:"<<std::endl;
    std::cin >> num_of_test;

    // 防止用户输错
    bool flag = true;
    while(flag){
        std::cout<<"是否测试乘除运算? 0-->否 1-->是"<<std::endl;
        std::cin >> level;
        if(level==0 || level==1)
            flag = false;
        else
            std::cout<<"请输入正确数值"<<std::endl;
    }

    // 相比于第一次提交新增加运算范围的选择 之前审题漏看了...
    flag = true;
    while(flag){
        std::cout<<"请选择运算的大小范围(0~100)"<<std::endl;
        std::cin>> range;
        if(range>0 || range<100)
            flag = false;
        else
            std::cout<<"请输入正确数值"<<std::endl;
    }

    std::cout<<"答题开始 !!!"<<std::endl;

    int count{1}; // 记录当前题号
    while(count<=num_of_test){

        printf("第%d题: \n",count);

        a = rand()%range+1; // 生成随机数
        b = rand()%range+1;

        // 随机生成运行类型 1:加法 2:减法 3:乘法 4:除法
        if(level==0)
            kind_of_test = rand()%2+1;
        else if (level==1)
            kind_of_test = rand()%4+1;

        switch (kind_of_test)
        {
            case 1:
                printf("%d + %d = ? (请通过键盘输入) \n",a,b);
                
                // Enter()返回用户输入
                c = Enter(total_time, min_time);
                
                right_answer = a+b;

                if(c==right_answer){
                    num_of_right++;
                    std::cout<<"恭喜你答对了!"<<std::endl;
                }else{
                    num_of_wrong++;
                    std::cout<<"哎呀,你答错了..."<<std::endl;
                    // 答错保存错题
                    SaveAnswer(count, kind_of_test, a, b, c, right_answer);
                }
                break;
            
            case 2:
                printf("%d - %d = ? (请通过键盘输入) \n",a,b);

                c = Enter(total_time, min_time);

                right_answer = a-b;

                if(c==right_answer){
                    num_of_right++;
                    std::cout<<"恭喜你答对了!"<<std::endl;
                }else{
                    num_of_wrong++;
                    std::cout<<"哎呀,你答错了..."<<std::endl;
                    SaveAnswer(count, kind_of_test, a, b, c, right_answer);
                }
                break;

            case 3:
                printf("%d x %d = ? (请通过键盘输入) \n",a,b);

                c = Enter(total_time, min_time);

                right_answer = a*b;

                if(c==right_answer){
                    num_of_right++;
                    std::cout<<"恭喜你答对了!"<<std::endl;
                }else{
                    num_of_wrong++;
                    std::cout<<"哎呀,你答错了..."<<std::endl;
                    SaveAnswer(count, kind_of_test, a, b, c, right_answer);
                }
                break;
            
            case 4:
                printf("%d / %d = ? (只取整数) \n",a,b);

                c = Enter(total_time, min_time);

                // 把int强制类型转换成float或double再做除法
                right_answer = (int)((float)(a)/(float)(b));

                if(c==right_answer){
                    num_of_right++;
                    std::cout<<"恭喜你答对了!"<<std::endl;
                }else{
                    num_of_wrong++;
                    std::cout<<"哎呀,你答错了..."<<std::endl;
                    SaveAnswer(count, kind_of_test, a, b, c, right_answer);
                }
                break;
        }
      
        count++;
    }

    std::cout<<std::endl<<"恭喜您完成训练 !!!"<<std::endl;

    // 结果输出

    // 这里必须强制类型转换成float或double int型不能做除法
    int score = (float)(num_of_right)/(float)(num_of_test) * 100.0f;
    printf("您一共答对%d, 答错%d, 得分:%d \n",num_of_right,num_of_wrong,score);

    printf("平均答题用时:%fs, 最短用时:%fs \n",total_time/num_of_test,min_time);
    
    // 如果不是满分 给出答错的题目与其正确答案
    if(score != 100){
        std::cout<<"其中: "<<std::endl;
        for(int i=0; i<wrong_answer_queue.size(); ++i){
            switch (wrong_answer_queue[i].kind_of_operation)
            {
            case 1:
                printf("第%d题: %d + %d, 正确答案为: %d, 您的答案为:%d \n",
                    wrong_answer_queue[i].num,
                    wrong_answer_queue[i].a,
                    wrong_answer_queue[i].b,
                    wrong_answer_queue[i].right_answer,
                    wrong_answer_queue[i].user_answer);
                break;
            
            case 2:
                printf("第%d题: %d - %d, 正确答案为: %d, 您的答案为:%d \n",
                    wrong_answer_queue[i].num,
                    wrong_answer_queue[i].a,
                    wrong_answer_queue[i].b,
                    wrong_answer_queue[i].right_answer,
                    wrong_answer_queue[i].user_answer);
                break;
            
            case 3:
                printf("第%d题: %d x %d, 正确答案为: %d, 您的答案为:%d \n",
                    wrong_answer_queue[i].num,
                    wrong_answer_queue[i].a,
                    wrong_answer_queue[i].b,
                    wrong_answer_queue[i].right_answer,
                    wrong_answer_queue[i].user_answer);
                break;

            case 4:
                printf("第%d题: %d / %d, 正确答案为: %d(只取整数), 您的答案为:%d \n",
                    wrong_answer_queue[i].num,
                    wrong_answer_queue[i].a,
                    wrong_answer_queue[i].b,
                    wrong_answer_queue[i].right_answer,
                    wrong_answer_queue[i].user_answer);
                break;
            }
        }
    }
    
}
