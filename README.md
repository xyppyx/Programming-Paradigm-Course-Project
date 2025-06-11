# 程序设计范式项目

## 项目概述

本项目是一个程序设计范式课程的学习项目，包含四次课程作业和两个综合性课程项目。通过不同的编程范式实现各种算法和应用，展示面向对象,函数式编程等范式的特点和应用场景。

## 项目结构

```
ProgrammingParadigm/
├── README.md                    # 项目说明文档
├── Homework/                    # 课程作业目录
│   ├── Assignment1/             # 第一次作业
│   ├── Assignment2/             # 第二次作业
│   ├── Assignment3/             # 第三次作业
│   └── Assignment4/             # 第四次作业
├── CourseProject/               # 课程项目目录
    ├── Gomoku/                  # 基于PVS算法的五子棋AI项目
    └── GenShinImpact/           # 基于UE5的类原神开放世界游戏Demo

```

## 项目内容

### 课程作业 (Homework)

包含四次课程作业，每次作业都专注于特定的编程范式概念

### 课程项目 (CourseProject)

#### 1. 五子棋AI项目 (Gomoku)

**项目简介:**
基于Principal Variation Search (PVS)算法的智能五子棋程序，采用高效的棋型识别系统和Zobrist哈希优化，实现了强大的AI对弈能力。

**核心技术特点:**

- **PVS搜索算法**: 基于Alpha-Beta剪枝的改进版本，通过主变例搜索提高搜索效率
- **Zobrist哈希**: 使用64位哈希值快速识别重复局面，配合置换表优化搜索
- **智能棋型识别**:
  - 预处理65536种棋型模式，O(1)时间复杂度识别棋型
  - 支持五连、活四、冲四、活三、眠三等9种棋型分类
  - 动态评估组合棋型（双活三、活三+冲四等）
- **启发式搜索优化**:
  - 优先搜索高价值落子点
  - 基于棋子密度的候选点筛选
  - 迭代加深搜索策略
- **时间控制**: 严格的1.4秒时间限制，包含超时保护机制

**算法核心指标:**

- 搜索深度: 3-7层迭代加深
- 棋盘规格: 12×12标准棋盘
- 候选落子: 每层最多15个候选点
- 时间限制: 1400ms
- 哈希表: 支持精确值、上界、下界三种类型

**技术实现亮点:**

1. **高效的棋型表系统**:

   ```cpp
   // 棋型表预处理，支持O(1)查询
   int g_pattern_table[65536][2];           // 65536=2^16种棋型
   double g_pattern_to_score[9][9][9][9];   // 四方向棋型组合评分
   ```
2. **Zobrist哈希优化**:

   ```cpp
   ull g_zobrist_board[kBoardSize + 8][kBoardSize + 8][2];  // 哈希表
   std::unordered_map<ull, HashEntry> g_zobrist_hashtable;  // 置换表
   ```
3. **PVS搜索算法**:

   ```cpp
   // PVS核心：先用空窗搜索，失败后重新全窗搜索
   if(find_pv) {
       score = -AlphaBeta(depth - 1, -alpha - 1, -alpha, enemy_color);
       if(alpha < score && score < beta)
           score = -AlphaBeta(depth - 1, -beta, -alpha, enemy_color);
   }
   ```

**程序运行模式:**

- **提交模式**: 基于课程要求格式，支持START/PLACE/TURN/END指令
- **调试模式**: 可视化棋盘显示，性能统计，手动落子测试
- **自动模式**: AI自动对弈，用于算法测试和调优

**编程范式体现:**

- **结构化编程**: 模块化的函数设计，清晰的逻辑层次
- **数据抽象**: Node结构体封装棋盘状态，Position结构体表示落子点
- **算法优化思维**: 位运算优化、查表法、剪枝策略等高效算法技巧

**技术栈:**

- C++ (由于测试平台限制,采用面向过程编程风格)
- STL容器和算法库
- 位运算和哈希算法
- 搜索算法和博弈论

**性能表现:**

- 平均搜索时间: 800-1200ms
- 搜索节点数: 10^4 - 10^6 级别
- 棋力水平: 业余高段水平


#### 2. 开放世界游戏Demo

**项目特点:**

- 基于Unreal Engine 5开发
- C++与蓝图混合编程
- 开放世界游戏机制
- 现代游戏开发工作流

**技术栈:**

- Unreal Engine 5
- C++
- Blueprint Visual Scripting
- 现代渲染技术

**核心功能:**

- 角色控制系统
- 世界生成系统
- 物理交互
- 用户界面系统

**仓库链接**

[https://github.com/feixiangdaihui/GenshinImpact](https://github.com/feixiangdaihui/GenshinImpact)

**个人贡献 - 敌人系统架构设计与实现:**

基于面向对象设计原则和组件化架构，设计并实现了一套完整的敌人系统。该系统采用三层架构模式，实现了高度的模块化和可扩展性。

**系统架构设计:**

```
敌人系统三层架构
├── 控制层 (Control Layer)
│   ├── EnemyAIController (接口)         # AI控制器基类接口
│   │   ├── BP_BossAIController          # Boss AI控制器实现
│   │   └── BP_CommonAIController        # 普通敌人AI控制器实现
│   ├── EnemyAnim (接口)                 # 敌人动画基类接口
│       └── ABP_E_Bear/Wolf...           # 熊/狼等敌人动画蓝图
├── 封装层 (Encapsulation Layer)
│   └── EnemyCharacter                   # 敌人角色封装类
│       ├── 基本属性 (位置、速度、生命值)
│       ├── 基本方法 (移动、攻击、检测)
│       └── 组件接口交互
└── 实现层 (Implementation Layer)
    ├── 检测系统
    │   ├── Detect Interface             # 检测接口
    │   └── Detect Component             # 检测组件实现
    ├── 移动系统
    │   ├── Move Interface               # 移动接口
    │   ├── Move Component               # 基础移动组件
    │   └── Immobile MoveComponent       # 静止型移动组件
    ├── 攻击系统
    │   ├── Attack Interface             # 攻击接口
    │   ├── Attack Component             # 基础攻击组件
    │   ├── Remote AttackComponent       # 远程攻击组件
    │   └── Skill AttackComponent        # 技能攻击组件
    └── 生命系统
        ├── Health Interface             # 生命接口
        ├── Health Component             # 基础生命组件
        ├── Rebound HealthComponent      # 反弹伤害组件
        └── Boss HealthComponent         # Boss生命组件
```

![敌人系统三层架构](CourseProject\GenshinImpact\敌人类的设计.png)

**设计模式应用:**

1. **接口隔离原则 (Interface Segregation Principle)**:

   - 将敌人行为分解为四个独立接口：检测、移动、攻击、生命
   - 每个接口只包含相关的方法，避免接口污染
2. **组件模式 (Component Pattern)**:

   - 每个功能模块都有对应的组件实现
   - 支持运行时动态组合不同组件创建多样化敌人
3. **策略模式 (Strategy Pattern)**:

   - 不同的AI控制器实现不同的行为策略
   - 可以在运行时切换敌人的AI行为
4. **工厂模式应用**:

   - 通过组件组合创建不同类型的敌人实例
   - 支持配置驱动的敌人生成

**核心组件实现特点:**

```cpp
// 控制层 - AI控制器接口设计
class EnemyAIController : public AAIController
{
public:
    virtual void UpdateAIBehavior() = 0;        // 纯虚函数，子类必须实现
    virtual void SetTargetPlayer(APawn* Player) = 0;
    virtual void OnEnemyDeath() = 0;
};

// 封装层 - 敌人角色类
class EnemyCharacter : public ACharacter
{
private:
    // 组件接口引用
    TScriptInterface<IDetectInterface> DetectComponent;
    TScriptInterface<IMoveInterface> MoveComponent;
    TScriptInterface<IAttackInterface> AttackComponent;
    TScriptInterface<IHealthInterface> HealthComponent;
  
public:
    // 基本属性
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxHealth = 100.0f;
  
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovementSpeed = 300.0f;
  
    // 通过接口调用具体实现
    void PerformDetection() { DetectComponent->Detect(); }
    void PerformMovement() { MoveComponent->Move(); }
    void PerformAttack() { AttackComponent->Attack(); }
};

// 实现层 - 组件接口示例
UINTERFACE(MinimalAPI, Blueprintable)
class UDetectInterface : public UInterface
{
    GENERATED_BODY()
};

class IDetectInterface
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintImplementableEvent)
    virtual void Detect() = 0;
  
    UFUNCTION(BlueprintImplementableEvent)
    virtual bool IsPlayerInRange(float Range) = 0;
};
```

**系统优势:**

1. **高度模块化**: 每个功能组件独立实现，便于单独测试和维护
2. **易于扩展**: 新增敌人类型只需组合现有组件或添加新组件
3. **代码复用**: 组件可以在不同敌人类型间复用
4. **职责清晰**: 每层都有明确的职责边界
5. **支持多态**: 通过接口实现运行时多态行为

**编程范式体现:**

- **面向对象编程**: 通过类继承和多态实现不同敌人行为
- **组件化编程**: 将复杂的敌人行为分解为可组合的组件
- **接口编程**: 依赖抽象而非具体实现，提高系统灵活性
- **策略模式**: 不同AI控制器实现不同的决策策略

**实际应用效果:**

通过这套架构，成功实现了:

- 普通敌人：基础移动 + 近战攻击 + 基础生命
- 远程敌人：基础移动 + 远程攻击 + 基础生命
- 技能敌人：基础移动 + 技能攻击 + 基础生命
- Boss敌人：复杂移动 + 多种攻击 + Boss生命系统
- 静态敌人：静止移动 + 反弹攻击 + 反弹生命

该系统设计充分体现了软件工程中的SOLID原则，为游戏的敌人系统提供了强大的扩展性和维护性基础。

## 编程范式应用

### 程序式编程范式

在五子棋项目中的体现:

- **模块化设计**: 将复杂的AI算法分解为独立的函数模块
- **数据驱动**: 使用全局数组和查找表实现高效的棋型识别
- **算法优化**: 通过位运算、哈希算法等底层优化技术提升性能

### 面向对象编程 (OOP)

- 在UE5项目中体现：Actor系统、组件化架构
- 结构化数据封装：Node、Position、HashEntry等结构体设计

### 函数式编程特性

- Lambda表达式在算法中的应用
- 纯函数设计：棋型识别和评分函数
- 递归算法：Alpha-Beta搜索的递归实现

## 使用指南

### 环境要求

**五子棋项目:**

- C++11 或更高版本
- 支持STL的编译器 (GCC/Clang/MSVC)
- 内存需求: 至少256MB可用内存

**UE5项目:**

- Unreal Engine 5.0+
- Visual Studio 2019/2022
- Windows 10/11

### 快速开始

```bash
# 编译五子棋项目
cd CourseProject/Gomoku
g++ -O2 -std=c++11 PVS.cpp -o Gomoku

# 运行调试模式
./Gomoku
# 输入: START 1 (执黑棋)

# 运行标准对局模式
# 将DEBUG_MODE设为0重新编译后运行
```

### 五子棋AI使用说明

**标准对局协议:**

```
START <color>    # 开始游戏，color: 1-黑棋，2-白棋
PLACE <x> <y>    # 对手落子位置
TURN             # 轮到AI落子
END <result>     # 游戏结束
```

**调试模式特殊指令:**

```
pb <x> <y>       # 手动放置黑棋
pw <x> <y>       # 手动放置白棋
pe <x> <y>       # 移除指定位置棋子
```

## 学习成果

通过本项目的学习和实践，掌握了：

1. **高级搜索算法**: PVS、Alpha-Beta剪枝、迭代加深等算法的实现和优化
2. **数据结构设计**: 哈希表、位运算、查找表等高效数据结构的应用
3. **算法工程化**: 时间控制、内存管理、性能优化等工程实践
4. **博弈论应用**: 极大极小算法、局面评估、搜索剪枝等博弈算法
5. **程序设计范式**: 从算法角度理解不同编程范式的适用场景和优势
