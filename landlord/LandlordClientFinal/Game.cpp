#include "Game.h"
#include "ui_Game.h"

Game::Game(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Game)
{
    ui->setupUi(this);
}

void Game::TimeCount(QString timeCount)
{
    this->ui->labelTimerCount->setText(timeCount);
}
void Game::clearTable()
{
    QList<Card*>childWidgets=this->ui->HandOutMy->findChildren<Card*>(QString(),Qt::FindDirectChildrenOnly);
    foreach(Card*cardt,childWidgets)
    {
        delete cardt;
    }
    childWidgets=this->ui->HandOutLeft->findChildren<Card*>(QString(),Qt::FindDirectChildrenOnly);
    foreach(Card*cardt,childWidgets)
    {
        delete cardt;
    }
    childWidgets=this->ui->HandOutRight->findChildren<Card*>(QString(),Qt::FindDirectChildrenOnly);
    foreach(Card*cardt,childWidgets)
    {
        delete cardt;
    }
    childWidgets=this->ui->ThreeCardShowAera->findChildren<Card*>(QString(),Qt::FindDirectChildrenOnly);
    foreach(Card*cardt,childWidgets)
    {
        delete cardt;
    }
    for(Card*card:std::as_const(Hand))
    {
        delete card;
    }
    Hand.clear();
    choosed.clear();
    received.clear();
    this->count_giveup=0;
    this->last=CardCombination(CardCombination::type::begin,0,0);
}
void Game::showInitCardNum(int landlordNum)
{
    if(landlordNum==this->mMyNum)//中
    {
        this->ui->labelCardNumMy->setText("20");
        this->ui->labelCardNumLeft->setText("17");
        this->ui->labelCardNumRight->setText("17");

    }
    else if(landlordNum==this->mMyNum+1||landlordNum==this->mMyNum-2)//右
    {
        this->ui->labelCardNumMy->setText("17");
        this->ui->labelCardNumLeft->setText("17");
        this->ui->labelCardNumRight->setText("20");
    }
    else if(landlordNum==this->mMyNum-1||landlordNum==this->mMyNum+2)//左
    {
        this->ui->labelCardNumMy->setText("17");
        this->ui->labelCardNumLeft->setText("20");
        this->ui->labelCardNumRight->setText("17");
    }
}
Game::~Game()
{
    delete ui;
}
void Game::showThreeCard(Card*card)
{
    card->setParent(this->ui->ThreeCardShowAera);
    this->ui->ThreeCardShowAera->layout()->addWidget(card);
}
void Game::GetHandCard(Card*card)
{
    this->Hand.append(card);
    card->setParent(this->ui->CardAera);

}
void Game::showMyCard()
{
    //this->ui->CardAera->layout()->removeWidget()
    this->sortHand();
    for(int i=0;i<this->Hand.size();i++)
    {
        this->ui->CardAera->layout()->addWidget(Hand[i]);
    }
}
void Game::removeCard(Card*card)
{
    if (!card) return; // 防止空指针崩溃
    this->ui->CardAera->layout()->removeWidget(card);
    card->deleteLater(); // 安全销毁，避免立即删除导致的崩溃
}
void Game::showHandOutCard(int who,QList<Card*>array)
{
    if(who==this->mMyNum)//中
    {
        QList<Card*>childWidgets=this->ui->HandOutMy->findChildren<Card*>(QString(),Qt::FindDirectChildrenOnly);
        foreach(Card*cardt,childWidgets)
        {
            delete cardt;
        }
        foreach(Card* card,array)
        {
            card->setParent(this->ui->HandOutMy);
            this->ui->HandOutMy->layout()->addWidget(card);
        }
        int num=this->ui->labelCardNumMy->text().toInt();
        num-=array.size();
        this->ui->labelCardNumMy->setText(QString::number(num));

    }
    else if(who==this->mMyNum+1||who==this->mMyNum-2)//右
    {
        QList<Card*>childWidgets=this->ui->HandOutRight->findChildren<Card*>(QString(),Qt::FindDirectChildrenOnly);
        foreach(Card*cardt,childWidgets)
        {
            delete cardt;
        }
        foreach(Card* card,array)
        {
            card->setParent(this->ui->HandOutRight);
            this->ui->HandOutRight->layout()->addWidget(card);
        }
        int num=this->ui->labelCardNumRight->text().toInt();
        num-=array.size();
        this->ui->labelCardNumRight->setText(QString::number(num));
    }
    else if(who==this->mMyNum-1||who==this->mMyNum+2)//左
    {
        QList<Card*>childWidgets=this->ui->HandOutLeft->findChildren<Card*>(QString(),Qt::FindDirectChildrenOnly);
        foreach(Card*cardt,childWidgets)
        {
            delete cardt;
        }
        foreach(Card* card,array)
        {
            card->setParent(this->ui->HandOutLeft);
            this->ui->HandOutLeft->layout()->addWidget(card);
        }
        int num=this->ui->labelCardNumLeft->text().toInt();
        num-=array.size();
        this->ui->labelCardNumLeft->setText(QString::number(num));
    }
}
void Game::showName_Image(QString info)//格式who;name;imagePath
{
    QStringList parts = info.split(";", Qt::KeepEmptyParts);
    int who=parts[0].toInt();
    QString name=parts[1];
    QString imagePath=parts[2];
    if(who==this->mMyNum)//中
    {
        if(!name.isEmpty())
        {
            this->ui->labelNameMy->setText(name);
        }
        else
        {
            this->ui->labelNameMy->setText(QString::number(who)+"号");
        }
        if(QFileInfo::exists(imagePath))
        {
            this->ui->picOtherMy->setIconSize(this->ui->picOtherLeft->size());
            this->ui->picOtherMy->setIcon(QIcon(imagePath));
        }
    }
    else if(who==this->mMyNum+1||who==this->mMyNum-2)//右
    {
        if(!name.isEmpty())
        {
            this->ui->labelNameRight->setText(name);
        }
        else
        {
            this->ui->labelNameRight->setText(QString::number(who)+"号");
        }
        if(QFileInfo::exists(imagePath))
        {
            this->ui->picOtherRight->setIconSize(this->ui->picOtherLeft->size());
            this->ui->picOtherRight->setIcon(QIcon(imagePath));
        }

    }
    else if(who==this->mMyNum-1||who==this->mMyNum+2)//左
    {
        if(!name.isEmpty())
        {
            this->ui->labelNameLeft->setText(name);
        }
        else
        {
            this->ui->labelNameLeft->setText(QString::number(who)+"号");
        }
        if(QFileInfo::exists(imagePath))
        {
            this->ui->picOtherLeft->setIconSize(this->ui->picOtherLeft->size());
            this->ui->picOtherLeft->setIcon(QIcon(imagePath));
        }
    }
}
bool greaterSort(const Card* c1, const Card* c2) {
    if (c1->rank != c2->rank) {
        return c1->rank> c2->rank;
    } else {
        return c1->suit> c2->suit;
    }
}
void Game::sortHand()
{
    std::sort(this->Hand.begin(),this->Hand.end(),greaterSort);
}
void Game::startConquerLandlord()
{
    this->ui->buttonHandOut->setEnabled(false);
}
void Game::disableLandlordButton()
{
    disconnect(this->ui->buttonGetLandlord,&QPushButton::clicked,this,&Game::getLandlord);
    disconnect(this->ui->buttonGiveUp,&QPushButton::clicked,this,&Game::giveUpLandlord);
    this->ui->buttonGetLandlord->setEnabled(false);
    this->ui->buttonGiveUp->setEnabled(false);
}
void Game::enableLandlordButton()
{
    this->ui->buttonGetLandlord->setEnabled(true);
    this->ui->buttonGiveUp->setEnabled(true);
    connect(this->ui->buttonGetLandlord,&QPushButton::clicked,this,&Game::getLandlord);
    connect(this->ui->buttonGiveUp,&QPushButton::clicked,this,&Game::giveUpLandlord);
}

void Game::startHandOut()
{
    this->ui->buttonGetLandlord->setEnabled(false);
}
void Game::disableHandOutButton()
{
    disconnect(this->ui->buttonHandOut,&QPushButton::clicked,this,&Game::handOut);
    disconnect(this->ui->buttonGiveUp,&QPushButton::clicked,this,&Game::giveUpHandOut);
    this->ui->buttonHandOut->setEnabled(false);
    this->ui->buttonGiveUp->setEnabled(false);
}
void Game::enableHandOutButton()
{
    this->ui->buttonHandOut->setEnabled(true);
    this->ui->buttonGiveUp->setEnabled(true);
    connect(this->ui->buttonHandOut,&QPushButton::clicked,this,&Game::handOut);
    connect(this->ui->buttonGiveUp,&QPushButton::clicked,this,&Game::giveUpHandOut);
}



bool isStraight(QList<Card*> array) {
    int size = array.size();
    // 条件1：至少5张，且最大rank不超过15（排除大小王，你的rank=16是小王、17是大王）
    if (size < 5 || array[0]->rank > 15) {
        return false;
    }
    // 条件2：降序连续（后一张rank = 前一张rank - 1），无重复牌
    for (int i = 0; i < size - 1; i++) {
        // 检查是否连续，且无重复（如两张3会导致rank相等，不满足顺子）
        if (array[i+1]->rank != array[i]->rank - 1) {
            return false;
        }
    }
    return true;
}
bool isStraightPair(QList<Card*> array) {
    int size = array.size();
    // 条件1：至少6张，偶数张，最大rank不超过15（无大小王）
    if (size < 6 || size % 2 != 0 || array[0]->rank > 15) {
        return false;
    }
    // 条件2：每两张为一对（同rank）
    for (int i = 0; i < size; i += 2) {
        if (array[i]->rank != array[i+1]->rank) {
            return false;
        }
    }
    // 条件3：对与对之间降序连续（下一对rank = 上一对rank - 1）
    for (int i = 0; i < size - 2; i += 2) {
        if (array[i+2]->rank != array[i]->rank - 1) {
            return false;
        }
    }
    return true;
}
// 适配你的Card类：判断降序排列的牌组是否为普通飞机（仅3张组，无带牌）
// threeNum：飞机的“组数”（如2组=6张，3组=9张）
bool isSimplePlane(QList<Card*> array, int threeNum, int& value) {
    int size = array.size();
    // 条件1：总张数 = 3 * 组数（如2组=6张），最大rank不超过15（无大小王）
    if (size != 3 * threeNum || array[0]->rank > 15) {
        return false;
    }
    // 条件2：每组3张牌rank相同
    for (int i = 0; i < size; i += 3) {
        if (array[i]->rank != array[i+2]->rank) {
            return false;
        }
    }
    // 条件3：各组之间降序连续（下一组rank = 上一组rank - 1）
    for (int i = 0; i < size - 3; i += 3) {
        if (array[i+3]->rank != array[i]->rank - 1) {
            return false;
        }
    }
    // 基准值为最大的三张组rank（降序排列，首组即最大）
    value = array[0]->rank;
    return true;
}
// 适配你的Card类：判断降序排列的牌组是否为飞机带单
// threeNum：飞机组数；value：返回飞机的基准rank（最大的三张组rank）
bool isSinglePlane(QList<Card*> array, int threeNum, int& value) {
    int size = array.size();
    // 条件1：总张数 = 3*组数 + 1*组数（带的单张数=组数）
    if (size != 4 * threeNum || array[0]->rank > 15) {
        return false;
    }

    // 统计每个rank的出现次数（仅用循环，不依赖QMap）
    QList<int> rankList; // 存储所有rank（去重）
    QList<int> countList; // 对应rank的出现次数
    for (Card* c : array) {
        int idx = rankList.indexOf(c->rank);
        if (idx == -1) {
            rankList.append(c->rank);
            countList.append(1);
        } else {
            countList[idx]++;
        }
    }

    // 条件2：有且仅有 threeNum 个rank出现3次（三张组），其余出现1次（单张）
    int threeCount = 0;
    int singleCount = 0;
    for (int cnt : std::as_const(countList)) {
        if (cnt == 3) threeCount++;
        else if (cnt == 1) singleCount++;
        else return false; // 出现2次或4次，不符合
    }
    if (threeCount != threeNum || singleCount != threeNum) {
        return false;
    }

    // 条件3：三张组的rank降序连续
    QList<int> threeRanks;
    for (int i = 0; i < rankList.size(); i++) {
        if (countList[i] == 3) {
            threeRanks.append(rankList[i]);
        }
    }
    for (int i = 0; i < threeRanks.size() - 1; i++) {
        if (threeRanks[i+1] != threeRanks[i] - 1) {
            return false;
        }
    }

    // 记录飞机基准值（最大的三张组rank）
    value = threeRanks[0];
    return true;
}
bool isPairPlane(QList<Card*> array, int threeNum, int& value) {
    int size = array.size();
    // 总张数必须为 3*组数 + 2*组数（每组三张带一对）
    if (size != 5 * threeNum || array[0]->rank > 15) { // 排除大小王（rank>15）
        return false;
    }

    // 1. 统计每个rank的出现次数（不依赖QMap，纯循环实现）
    QList<int> rankList;  // 存储去重后的rank（按降序排列，因原数组已降序）
    QList<int> countList; // 对应rank的出现次数
    for (Card* c : array) {
        int idx = rankList.indexOf(c->rank);
        if (idx == -1) {
            rankList.append(c->rank);
            countList.append(1);
        } else {
            countList[idx]++;
        }
    }

    // 2. 验证数量：必须有 threeNum 个rank出现3次（三张组），threeNum 个rank出现2次（对子）
    int threeCount = 0; // 三张组的数量
    int pairCount = 0;  // 对子的数量
    for (int cnt : countList) {
        if (cnt == 3) {
            threeCount++;
        } else if (cnt == 2) {
            pairCount++;
        } else {
            return false; // 出现1次或4次等无效数量，排除
        }
    }
    if (threeCount != threeNum || pairCount != threeNum) {
        return false;
    }

    // 3. 提取三张组的rank，并验证是否降序连续
    QList<int> threeRanks;
    for (int i = 0; i < rankList.size(); i++) {
        if (countList[i] == 3) {
            threeRanks.append(rankList[i]);
        }
    }
    // 检查三张组是否连续递减（适配降序）
    for (int i = 0; i < threeRanks.size() - 1; i++) {
        if (threeRanks[i+1] != threeRanks[i] - 1) {
            return false;
        }
    }

    // 4. 记录飞机基准值（最大的三张组rank）
    value = threeRanks[0];
    return true;
}
CardCombination judgeCard(QList<Card*> array) {
    CardCombination temp(CardCombination::type::begin, 0, 0);
    int value = 0;
    int cardSize = array.size();

    // 提前处理：王炸（优先级最高，避免被其他2张牌逻辑覆盖）
    if (cardSize == 2) {
        if (array[0]->rank == Card::Rank::BJOKER && array[1]->rank == Card::Rank::SJOKER) {
            temp.changeContent(CardCombination::type::Bomb, 2, Card::Rank::BJOKER); // 王炸用大王rank标识
            return temp;
        }
    }
    bool isThreeTwo=false;
    bool isFourTwo=false;
    switch (cardSize) {
    case 1: // 单张（降序排列，直接取唯一rank）
        temp.changeContent(CardCombination::type::Single, 1, array[0]->rank);
        break;

    case 2: // 对子（排除王炸，已提前处理）
        if (array[0]->rank == array[1]->rank && array[0]->rank <= 15) { // 排除大小王
            temp.changeContent(CardCombination::type::Pair, 2, array[0]->rank);
        }
        break;

    case 3: // 三张（降序排列，3张rank相同）
        if (array[0]->rank == array[1]->rank && array[1]->rank == array[2]->rank && array[0]->rank <= 15) {
            temp.changeContent(CardCombination::type::Three, 3, array[0]->rank);
        }
        break;

    case 4: // 炸弹 / 三带一（降序排列，同值牌靠在一起）
        // 炸弹：4张rank相同（排除王炸）
        if (array[0]->rank == array[3]->rank && array[0]->rank <= 15) {
            temp.changeContent(CardCombination::type::Bomb, 4, array[0]->rank); // 修复原函数用array[1]->rank的错误
        }
        // 三带一：前3张同 或 后3张同
        else if ((array[0]->rank == array[2]->rank) || (array[1]->rank == array[3]->rank)) {
            int threeRank = (array[0]->rank == array[2]->rank) ? array[0]->rank : array[1]->rank;
            temp.changeContent(CardCombination::type::Three_One, 4, threeRank); // 修复原函数用array[1]->rank的潜在错误
        }
        break;

    case 5: // 三带二 / 顺子（降序排列，优先判断三带二，再判断顺子）
        // 三带二：前3张同+后2张同 或 前2张同+后3张同
        isThreeTwo = (array[0]->rank == array[2]->rank && array[3]->rank == array[4]->rank)
                          || (array[0]->rank == array[1]->rank && array[2]->rank == array[4]->rank);
        if (isThreeTwo && array[0]->rank <= 15) {
            int threeRank = (array[0]->rank == array[2]->rank) ? array[0]->rank : array[2]->rank;
            temp.changeContent(CardCombination::type::Three_two, 5, threeRank); // 修复原函数用Three_One的错误
        }
        break;

    case 6: // 四带二 / 普通飞机（2组三张） / 连对（3对）
        // 四带二：4张同+2张同（降序排列，4张同的牌在前或在后）
        isFourTwo = (array[0]->rank == array[3]->rank && array[4]->rank == array[5]->rank)
                         || (array[0]->rank == array[1]->rank && array[2]->rank == array[5]->rank);
        if (isFourTwo && array[0]->rank <= 15) {
            int fourRank = (array[0]->rank == array[3]->rank) ? array[0]->rank : array[2]->rank;
            temp.changeContent(CardCombination::type::Four_Two, 6, fourRank); // 修复原函数size=5的错误

        }
        // 普通飞机（2组三张）
        else if (isSimplePlane(array, cardSize / 3,value)) { // cardSize/3=2组
            temp.changeContent(CardCombination::type::Plane, 6, value);
        }
        break;

    case 8: // 飞机带单（2组三张+2张单：3*2+1*2=8）
        if (isSinglePlane(array, cardSize / 4, value)) { // cardSize/4=2组
            temp.changeContent(CardCombination::type::PlaneSingle, 8, value);
        }
        break;

    case 10: // 飞机带对（2组三张+2对：3*2+2*2=10）
        if (isPairPlane(array, cardSize / 5, value)) { // cardSize/5=2组
            temp.changeContent(CardCombination::type::PlanePair, 10, value);
        }
        break;

    case 12: // 普通飞机（4组三张：3*4=12）
        if (isSimplePlane(array, cardSize / 3, value)) { // cardSize/3=4组
            temp.changeContent(CardCombination::type::Plane, 12, value);
        }
        break;

    case 15: // 普通飞机（5组三张：3*5=15）
        if (isSimplePlane(array, cardSize / 3, value)) { // cardSize/3=5组
            temp.changeContent(CardCombination::type::Plane, 15, value);
        }
        break;

    case 16: // 飞机带单（4组三张+4张单：3*4+1*4=16）
        if (isSinglePlane(array, cardSize / 4, value)) { // cardSize/4=4组
            temp.changeContent(CardCombination::type::PlaneSingle, 16, value);
        }
        break;

    default:
        break;
    }

    // 最后判断顺子/连对（无效牌型才进入，避免覆盖高优先级牌型）
    if (temp.t == CardCombination::type::begin) {
        // 顺子（5张及以上，降序连续）
        if (isStraight(array)) {
            temp.changeContent(CardCombination::type::Straight, cardSize, array[0]->rank);
        }
        // 连对（6张及以上偶数张，降序连续对）
        else if (isStraightPair(array)) {
            temp.changeContent(CardCombination::type::StraightPair, cardSize, array[0]->rank); // 修复原函数用Straight的错误
        }
    }

    return temp;
}
void Game::Changelast()
{
    this->last=judgeCard(this->received);
}
bool Game::outHand()
{
    this->choosed.clear();
    for(int i=0;i<this->Hand.size();i++)
    {
        if(this->Hand[i]->isSelected)
        {
            this->choosed.append(this->Hand[i]);
        }
    }
    if(choosed.length()==0)
    {
        return false;
    }
    else if(last.t==CardCombination::type::begin)
    {
        CardCombination now=judgeCard(choosed);
        if(now.t!=CardCombination::type::begin)
        {
            return true;
        }
    }
    if(choosed.length()==last.length)
    {
        CardCombination now=judgeCard(choosed);
        if(now.t==last.t&&now.value>last.value)
        {
            return true;
        }
    }
    else if(last.t!=CardCombination::type::Bomb&&choosed.length()==4)
    {
        CardCombination now=judgeCard(choosed);
        if(now.t==CardCombination::type::Bomb)
        {
            return true;
        }
    }
    else if(choosed.length()==2)
    {
        CardCombination now=judgeCard(choosed);
        if(now.t==CardCombination::type::Bomb)
        {
            return true;
        }
    }
    return false;
}

CardCombination& CardCombination::operator=(const CardCombination& combination)
{
    this->t=combination.t;
    this->length=combination.length;
    this->value=combination.value;
    return *this;
}
CardCombination::CardCombination(CardCombination::type t,int length,int value)
{
    this->t=t;
    this->length=length;
    this->value=value;
}
CardCombination::CardCombination(CardCombination& combin)
{
    this->t=combin.t;
    this->length=combin.length;
    this->value=combin.value;
}
void CardCombination::changeContent(CardCombination::type t,int length,int value)
{
    this->t=t;
    this->length=length;
    this->value=value;
}


