// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ItemUseAction/UA_Pistol.h"

void UUA_Pistol::InitUseAction(APlayerController* InLocalPC)
{
	Super::InitUseAction(InLocalPC);
}

void UUA_Pistol::ResetUseAction()
{
	Super::ResetUseAction();
}

void UUA_Pistol::HandleInputKey(const FGameplayTag& InKey, const FGameplayTag& InState)
{
	if (InKey == SETGameplayTag::GameInputKey::LB)
	{
		if (InState == SETGameplayTag::InputKeyState::Press)
		{
			// 开火逻辑,设置定时器,到权威生成子弹,同时消耗当前弹药
			// 走一个专门的weapontick component吗?
			// 开火,weapontick发个rpc到权威
			// 权威开始设置定时器,检测当前手持物品的状态和剩余量,决定能否发射子弹,
			// 所以还是需要一个uobject,这个uobject存在于权威,一个useactiob对应一个obj
			// obj提供虚函数OnceTick,返回bool,按引用接收playerquickbar和currentquickbarindex
			// OnceTick中先判断当前手持物品是否是所需的,比如当前就需要手持物品类型为Pistol
			// 验证成功,看看currentammo数量是否大于0,是,返回true
			// weapontick收到true后,真正向bulletmanager申请子弹,发射此时已经为权威,没问题
			// 停止射击也是从本地向weapontick申请,weapontick向服务器发rpc
			// 而对于部分效果,如没弹药开火音效,等视觉效果,则可以尝试直接本地读当前值预测
			// 但是弹药显示hud呢? 玩家手持枪械后,我希望右下角出现当前枪械图表,当前弹药数和背包内剩余弹药数
			// 我想要的是这个hud自己监听FOnHeldItemChanged,提供一个Idetailitem接口
			// FOnHeldItemChanged触发后,把当前的useactioncast到Idetailitem,cast成功,则表示这个物品需要信息界面
			// Idetailitem提供一个接收fondetailitemnumchangeddelegate
			// 信息panelcast成功后,构造一个传入,当useaction被tick后,对其调用,传入一个float,这个float就是detailpanel需要显示的值
			// 但是这依赖定时器在本地
			// 但实际射击和消耗弹药应该是在权威上跑,如果权威上射击成功,再发一个从权威到客户端的rpc吗?
			// 里面附带射击后剩余的弹药,由tickcomponent再转发给本地的useaction?
			// 那不如这样,tickcomponent在本地定时,发射子弹发rpc,本地定时器同时能tikc到useaction
			// useaction在每次被tick射击时,广播出去的值就是查询本地的当前剩余弹药数量减一,
			// 这个值是本地算出来的,当服务器实际修改值后,会复制回来,最终值会被服务器修正,也没问题
			// 但其实还有一个最简单的,就是detailpanel轮询,当cast到Idetailitem成功后,启动tick开始轮询,cast失败,关闭tick
			// 而这时候Idetailitem的接口就可以变成返回float的值,而useaction对这个接口的实现就是去查对应的数量并返回
			// 这样只有一个rpc,同时显示的数据又是最新的
			// 但又有问题了,查询需要找
			// UPROPERTY(EditAnywhere)
			// TArray<FItemStateEntry> StateEntry;
			// 这个array每帧找是有开销的,那就是FInventorySlot维护一个本地缓存,缓存tmap,因为tmap没办法复制
			// 这个缓存在库存发生变化时,只重建发生变化槽位的缓存
			// 需要每个实现了IStorable的NotifyInventoryItemChanged中实现,并且服务器上存在所有玩家的缓存
			// 或者这样也可以
			// Idetailitem返回一个gameplaytag,表示我想让detailpanel显示哪个属性值,
			// 目前FOnHeldItemChanged的触发只有两处,一是玩家切换,而是因为拖拽导致的触发,不管怎么说,只要出发了,就一定是当前手持物品发生了改变
			// 而这个提供tag的方案,需要监听手持物品的数量发生了改变
			// 这里还有个问题,如果玩家干的是使用当前手持物品,发射子弹,当前手持物品数量发生改变,触发OnInventoryItemChanged
			// 而OnInventoryItemChanged检测到是currenthbindex,又会反过来触发FOnHeldItemChanged,这个不对
			// 要区分开吗?但是不好区分,因为拖拽的最终结果也是导致触发OnInventoryItemChanged
			// 要有办法能够区分当前手持物品的整体变化和单个数据的变化
			// 用fguid也不行,两把一模一样的枪,快捷栏1托2,2进1,虽然两把枪一样,但是还是应该触发
			// 物品拆分就需要不断的分配fguid,但其实他们属于同一类型
			// 最好的方式还是想办法区分是因为使用导致的变化还是因为拖拽导致的变化

		}
	}
}

void UUA_Pistol::BreakAllInput()
{

}
