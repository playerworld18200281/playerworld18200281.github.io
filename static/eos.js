var vm = new Vue({
  el: '#app',
  data: {
    language:{
    	Chinese: {
          big:"大",
          small:"小",
          winningRules:"奖品池授予每24小时一次。对于每个新投资时，将时间延长3小时，以及100％的奖金池被奖励给玩家提供近年来最大的投资。小奖金池每24小时、1/3、1/3的三等奖。",
          ticket:"购票",
          participate:"参与",
          rate:"当前换算率",
          send:"发送ETH",
          referee:"我的裁判是：",
          push:"我的直接推送数量：",
          invitation:"我的邀请链接：",
          copy:"复制邀请链接",
          statistics:"统计数据",
          income:"我的收入",
          statistical:"统计",
          historical:"历史记录",
          dividendIncome:"安息红利收入",
          recommendIncome:"推荐奖励",
          pool:"奖池",
          countdown:"下一次收入倒计时",
          incomeLimits:"收入限制仍然存在",
          notStable:"收入不稳定",
          settle:"Settle业务收益",
          exitEarnings:"退出取消收入",
          exitIncome:"退出收益",
          rules:"比赛规则",
          rulesContext:"1、账户在退出前只能接受一项投资，并可在退出后继续投资静态股息每天波动0.3％至2.2％；<br />2、退出游戏，按1-10投资金额为3倍，11-30投资金额为4倍，大于31投资金额为5倍；<br />3、动态推荐25％根据：10％ - > 8％ - > 7％获得动态奖励，即时提现现金；<br />4、奖池达到指定时间后，奖池将不会结算，直到下一轮开放；<br />5、奖池达到规定时间后，不会存入，投资金额将直接计入股息收入；<br />6、如果不满足条件，则动态沉淀到红色池中。",
          bigPol:"大奖池当前总量",
          smallPol:"小奖池当前总量",
          ticketTate:"门票焚毁倍率",
          burned:"已经投入的总资金",
          total:"总结算分红收益",
          recommend:"总的推荐收益",
          Team:"团队",
          Ambassador:"大使",
          totalBig:"大奖池",
          totalSmall:"小奖池",
          totalSee:"查看我的统计数量",
          prompt:"提示内容<a href='http://www.baidu.com'>百度</a>",
          yep:"确认",

    	},
    	English: {
          big:"big",
          small:"small",
          winningRules:"The prize pool is awarded every 24 hours. For each new investment, the time is increased by 3 hours, and 100% of the bonus pool is awarded to the player to provide the biggest investment in recent years. Small bonus pool every 24 hours, 1/3, 1/3 of the third prize.",
          ticket:"ticket",
          participate:"Participate",
          rate:"Current conversion rate",
          send:"Send ETH",
          referee:"My referee is:",
          push:"My direct push number:",
          invitation:"My invitation link:",
          copy:"Copy invitation links",
          statistics:"statistics",
          income:"My income",
          statistical:"statistical",
          historical:"historical record",
          dividendIncome:"Settled dividend income",
          recommendIncome:"Referral bonuses",
          pool:"The prize pool",
          countdown:"Next income countdown",
          incomeLimits:"Income limit remains",
          notStable:"Unsettled income",
          settle:"Settle business gains",
          exitEarnings:"Withdrawal of income",
          exitIncome:"Extraction yield",
          rules:"The rules of the game",
          rulesContext:"1. The account can only accept one investment before the withdrawal, and can continue to invest in the static dividend after the withdrawal, which fluctuates by 0.3% to 2.2% every day; <br />2, exit the game, according to 1-10 investment amount is 3 times, 11-30 investment amount is 4 times, greater than 31 investment amount is 5 times; <br />3, dynamic recommendation 25% according to: 10% - > 8% - > 7% get dynamic reward, immediate cash withdrawal; 4. After the prize pool reaches the specified time, the prize pool will not be settled until the next round of opening; 5. After the bonus pool reaches the specified time, it will not be deposited, and the investment amount will be directly included in the dividend income; <br />6, if the condition is not met, it will be dynamically precipitated into the red pool.",
          bigPol:"Big pot",
          smallPol:"Small pot",
          ticketTate:"Ticket burnout rate",
          burned:"Has been burned",
          total:"Total settled",
          recommend:"Total recommended",
          Team:"Team",
          Ambassador:"Ambassador",
          totalBig:"Big pot",
          totalSmall:"Small pot",
          totalSee:"Check my stats",
          yep:"confirm",
          
      }
    },
    languageBlooen: localStorage.getItem("isLanguage") || "Chinese",
    languageCon:{},
    isNavActive:false,//导航语言选择框
    isActive:1,
    isBuy:true,
    amount:"",
    isActive2:1,
    isPrompt:false,
  },
  methods: {
    // 语言弹出框
    tabNav(){
      this.isNavActive = !this.isNavActive
    },
    // 切换语言
    languageTab(language){
      localStorage.setItem("isLanguage", language);
      console.log(window.localStorage)
      if (language == "Chinese") {
        this.languageCon = this.language.Chinese
      } else if(language == "English") {
        this.languageCon = this.language.English
      }
      this.isNavActive = false
    },
    // 购买/参与-切换
    switchFunction(t){
      this.isActive = t;
    },
    // 购买
    amountFun(num){
      this.amount = num
    },
    // 
    statistical(num){
      this.isActive2 = num;
    },
    // 退出收益按钮
    extract(){
      this.isPrompt = true
    },
    cancelY(){
      this.isPrompt = false
    }

  },
  mounted: function () {
    if (this.languageBlooen == "Chinese") {
        this.languageCon = this.language.Chinese
      } else if(this.languageBlooen == "English") {
        this.languageCon = this.language.English
      }
  },

})