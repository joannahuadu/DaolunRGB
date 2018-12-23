var app = getApp()
Page({

  /**
   * 页面的初始数据
   */
  data: {
    R:[],
    G: [],
    B: [],
    jiage1:1,
    jiage2:2,
    jiage3:3,
    sum:0
  },
  accout: function () {
    var length = app.globalData.R.datapoints.length
    for (var i = 0; i < length; i++) {
    ///  categories.push(app.globalData.humidity.datapoints[i].at.slice(11, 19));
      this.data.R[i]=app.globalData.R.datapoints[i].value;
      this.setData({
        R: this.data.R})
      this.data.G[i]=app.globalData.G.datapoints[i].value;
      this.setData({
        G: this.data.G
      })
      this.data.B[i]=app.globalData.B.datapoints[i].value;
      this.setData({
        B: this.data.B
      })
     
      console.log(app.globalData.R.datapoints[2])
      

      
    }
 
   // return {
     // categories: categories,
     // this.data.G:G,
    //  B: B,
   //   R: R
   // }
    },
    sumGet: function ()
    {
    
      var length = app.globalData.R.datapoints.length
      for (var i = 0; i < length; i++)

       {
        
        this.data. sum=this.data.sum+(parseInt([(this.data.R[i]+ 256) / (this.data.B[i] + 256)]) * parseInt([(this.data.R[i] + 256) / (this.data.G[i] + 256)]) * this.data.jiage1 + parseInt([(this.data.B[i] + 256) / (this.data.G[i] + 256)]) * parseInt([(this.data.B[i] + 256) / (this.data.R[i] + 256)]) * this.data.jiage3 + parseInt([(this.data.G[i] + 256) / (this.data.B[i] + 256)]) * parseInt([(this.data.G[i] + 256) / (this.data.R[i] + 256)]) * this.data.jiage2)
       
      //  console.log(sum)
        
    }
    
      this.setData({
        sum: this.data.sum
      })
    },

  click: function () {
    wx.scanCode({
      success(res) {
        console.log(res)
      }
    })
  },
   
    

 onLoad: function (options) 
  {},

    //this.setData({
      //R: app.globalData.R.datapoints[0].value,
     // G: app.globalData.G.datapoints[0].value,
     // B: app.globalData.B.datapoints[0].value,})},
   
    
   // })

  
  
  
  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  }
})