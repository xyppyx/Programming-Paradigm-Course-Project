#include<iostream>
#include<string>

using std::cin;
using std::cout;
using std::endl;
using std::cerr;

class User_Prof
{
    private://members
        static int m_user_count;
        std::string m_name;
        int m_id;
        int m_login_times;
        int m_played_matches;
        int m_won_matches;
        double m_won_rate;
        std::string m_rank;
    public://constructor
        User_Prof() : m_name("guest"), m_id(++m_user_count), m_login_times(0), m_played_matches(0), m_won_matches(0), m_won_rate(0.0), m_rank("beginner") {}
    
    public://external interfaces
        void Login();
        void Logout();
        bool TestEquivalence(const User_Prof &other_user);
        bool TestNotEquivalence(const User_Prof &other_user);
};

void User_Prof::Login()
{
    cout<<"Login successfully!"<<endl;
    m_login_times++;
}

void User_Prof::Logout()
{
    cout<<"Logout successfully!"<<endl;
}

bool User_Prof::TestEquivalence(const User_Prof &other_user)
{
    return m_id == other_user.m_id;
}

bool User_Prof::TestNotEquivalence(const User_Prof &other_user)
{
    return m_id != other_user.m_id;
}

int User_Prof::m_user_count = 0;

int main()
{
    User_Prof user1;
    User_Prof user2;
    user1.Login();
    user1.Logout();
    cout<<user1.TestEquivalence(user2)<<endl;
    cout<<user1.TestNotEquivalence(user2)<<endl;
    return 0;
}