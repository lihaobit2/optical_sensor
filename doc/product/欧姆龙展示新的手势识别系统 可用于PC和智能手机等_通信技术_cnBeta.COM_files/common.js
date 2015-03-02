var fixed_top;

;(function() {
    //全局ajax处理
    $.ajaxSetup({
        complete: function(jqXHR) {
            //登录失效处理
            if(typeof(jqXHR.responseText) != 'undefined'){
                if(jqXHR.responseText.state === 'logout') {
                    location.href = GV.URL.LOGIN;
                }
            }
        },
        data : {
            //csrf_token : GV.TOKEN
        },
        error : function(jqXHR, textStatus, errorThrown){
            //请求失败处理
            //alert(errorThrown ? errorThrown : '操作失败');
        }
    });
    var isIE=navigator.userAgent.toUpperCase().indexOf("MSIE")?true:false;
    if(isIE) {
        //ie 都不缓存
        $.ajaxSetup({
            cache : false
        });
    }

    //不支持placeholder浏览器下对placeholder进行处理
    if(document.createElement('input').placeholder !== '') {
        $('[placeholder]').focus(function() {
            var input = $(this);
            if(input.val() == input.attr('placeholder')) {
                input.val('');
                input.removeClass('placeholder');
            }
        }).blur(function() {
            var input = $(this);
            if(input.val() == '' || input.val() == input.attr('placeholder')) {
                input.addClass('placeholder');
                input.val(input.attr('placeholder'));
            }
        }).blur().parents('form').submit(function() {
            $(this).find('[placeholder]').each(function() {
                var input = $(this);
                if(input.val() == input.attr('placeholder')) {
                    input.val('');
                }
            });
        });
    }

    //导航
    var nav_arrow = $('.nav_arrow'),sub_navi = $('.sub_navi');
    nav_arrow.on('mouseenter', function(e){
        $(this).addClass('nav_arrow_hover');
        sub_navi.css({'height':'auto','overflow':'auto'});
    });
    sub_navi.on('mouseleave', function(e){
        nav_arrow.removeClass('nav_arrow_hover');
        sub_navi.css({'height':'28px','overflow':'hidden'});
    });

    //回到顶部的JS
    var back_top_btn = $('#backToTop');
    if (back_top_btn.length) {
        var $top = back_top_btn.click(function() {
                $("html,body").animate({
                    scrollTop: 0
                }, 120)
                return false;
            }),timer = null;

        $(window).bind("scroll", function() {
            var dTop = $(document).scrollTop(),
                wHeight = $(window).height();
            0 < dTop ? $top.show() : $top.hide();
        })
    }

    $.extend({
        getLength : function(q, g) {
            var trim = function(h) {
                    try {
                        return h.replace(/^\s+|\s+$/g, "")
                    } catch (j) {
                        return h
                    }
                },
                byteLength = function(b) {
                    if (typeof b == "undefined") {
                        return 0
                    }
                    var a = b.match(/[^\x00-\x80]/g);
                    return (b.length + (!a ? 0 : a.length))
                };

            g = g || {};
            g.max = g.max || 320;
            g.min = g.min || 41;
            g.surl = g.surl || 20;
            var p = trim(q).length;
            if (p > 0) {
                var j = g.min,
                    s = g.max,
                    b = g.surl,
                    n = q;
                var r = q.match(/(http|https):\/\/[a-zA-Z0-9]+(\.[a-zA-Z0-9]+)+([-A-Z0-9a-z\$\.\+\!\_\*\(\)\/\,\:;@&=\?~#%]*)*/gi) || [];
                var h = 0;
                for (var m = 0, p = r.length; m < p; m++) {
                    var o = byteLength(r[m]);
                    if (/^(http:\/\/t.cn)/.test(r[m])) {
                        continue
                    } else {
                        if (/^(http:\/\/)+(t.sina.com.cn|t.sina.cn)/.test(r[m]) || /^(http:\/\/)+(weibo.com|weibo.cn)/.test(r[m])) {
                            h += o <= j ? o : (o <= s ? b : (o - s + b))
                        } else {
                            h += o <= s ? b : (o - s + b)
                        }
                    }
                    n = n.replace(r[m], "")
                }
                return Math.ceil((h + byteLength(n)) / 2)
            } else {
                return 0
            }
        },
        htmlspecialchars : function(str) {
            var s = "";
            if (str.length == 0) return "";
            for(var i=0; i<str.length; i++){
                switch (str.substr(i,1)){
                    case "<": s += "&lt;"; break;
                    case ">": s += "&gt;"; break;
                    case "&": s += "&amp;"; break;
                    case " ":
                        if(str.substr(i + 1, 1) == " "){
                            s += " &nbsp;";
                            i++;
                        } else s += " ";
                        break;
                    case "\"": s += "&quot;"; break;
                    case "\n": s += "<br>"; break;
                    default: s += str.substr(i,1); break;
                }
            }
            return s;
        }
    });
    $.fn.extend({
		insertContent: function(myValue,t){
			var $t=$(this)[0];

			if (document.selection) {//ie
				this.focus();
				var sel = document.selection.createRange();
				sel.text = myValue;
				this.focus();
                sel.moveStart ('character', -l);
		        var wee = sel.text.length;
                if(arguments.length == 2){
                    var l = $t.value.length;
                    sel.moveEnd("character", wee+t );
                    t<=0?sel.moveStart("character",wee-2*t-myValue.length):sel.moveStart("character",wee-t-myValue.length);

                    sel.select();
                }
			} else if ($t.selectionStart || $t.selectionStart == '0') {
                var startPos = $t.selectionStart;
                var endPos = $t.selectionEnd;
                var scrollTop = $t.scrollTop;
                $t.value = $t.value.substring(0, startPos) + myValue + $t.value.substring(endPos, $t.value.length);
                this.focus();
                $t.selectionStart = startPos + myValue.length;
                $t.selectionEnd = startPos + myValue.length;
                $t.scrollTop = scrollTop;
                if(arguments.length == 2){
                    $t.setSelectionRange(startPos-t,$t.selectionEnd+t);
                    this.focus();
                }
            }
            else {
                this.value += myValue;
                this.focus();
            }
		}
    });
    // close user pop
	if( $('#J_popup_box').length ) {
        $('#J_popup_box').mouseover(function(){
            $('#J_popup_menu').show();
        }).mouseout(function(){
            $('#J_popup_menu').hide();
        });
    }
})();
var snsWindow = {
        OpenWindow:!1,
        extQq:[650, 480],
        extWb:[750, 555],
        extDb:[590, 435]
    },
    windowOpen = function(){
        var $this = $(this),
            href = $this.prop('href'),
            n='';

        href.indexOf("sina") >= 0 ? n = "extWb" : href.indexOf("qq") >= 0 ? n = "extQq" : href.indexOf("douban") >= 0 ? n = "extDb" : (t = "qq_weibo", n = "extQq");
        var r = snsWindow[n][0],
            i = snsWindow[n][1],
            s = (window.screen.width - r) / 2,
            o = (window.screen.height - i - 68) / 2;

        snsWindow.OpenWindow = window.open(href, "_blank", "width=" + r + "px, height=" + i + "px, left=" + s + "px, top=" + o + "px, directories=0, location=0, menubar=0, resizable=0, status=0, toolbar=0");

        return false;
    },
    loginCallBack = function(reload, udata){
        reload = arguments[0] ? arguments[0] : "0";
        udata = arguments[1] ? arguments[1] : "{}";
        snsWindow.OpenWindow && setTimeout(function() {
            typeof($.fancybox) === 'function' && $.fancybox.close();
            if(reload == '1'){
                window.location.href = window.location.href;
            }else if(reload == '2'){
                udata = $.parseJSON(udata);
                if(udata.ID != 'undefined'){
                    GV.USER = udata;
                }
                if($("#top_reply_login").length > 0 && GV.USER.ID > 0){
                    var login = $("#top_reply_login");
                    login.find("span").html('<a target="_blank" href="/user/'+GV.USER.ID+'" class="userName">'+GV.USER.NICK+'</a>');
                    login.find("img").attr("src",GV.USER.ICON);
                    login.show();
                    $("#top_reply_logout").hide();
                    if($("#favorite_btn").length > 0){
                        $("#favorite_btn").show();
                    }
                    if($("#post_sync").length > 0){
                        $("#post_sync").removeClass("none").find("input[name=post_sync]").attr("checked", false);
                        $("#post_sync").find(".sync_item").hide();
                        $("#post_sync").find("#"+GV.USER.TYPE+"_weibo").show();
                    }
                }
            }else{
                window.location = '/user';
            }
            snsWindow.OpenWindow.window.close(), snsWindow.OpenWindow = !1
        }, 1e3);
    };
var popLoginTp = '\
    <div class="cb-popLogin wrap">\
        <div class="login-connect">\
            <h3>使用合作网站帐号登录</h3>\
            <div class="connections clearfix">\
                <a tabindex="-1" href="/user/login/sina?cb=pop&re=2" class="weibo login-button">微博帐号登录</a>\
                <a tabindex="-1" href="/user/login/qq?cb=pop&re=2" class="qzone login-button">qq帐号登录</a>\
            </div>\
            <p class="less">可使用以上帐号直接登录</p>\
        </div>\
    </div>';
var cmt_tp = '\
        <dl id="J_Comment_Item_<%=tid%>">\
            <dt><strong class="re_lou"><%=lou%>楼</strong><span class="re_username"><%=name%></span><span class="re_area">来自[<%=host_name%>]</span><span class="datetime">发表于 <%=date%></span></dt>\
            <dd class="comment_avatars"><img width="48px" height="48px" src="<%=icon%>" /></dd>\
            <dd class="comment_body">\
                <div id="J_Comment_Quote_<%=tid%>"></div>\
                <div class="re_text"><%=comment%></div>\
                <div class="re_mark">\
                    <span action-tid="<%=tid%>">\
                        <a class="commt_a_link" supported="false" action-type="support" href="javascript:;" title="支持"><span>支持(<em><%=score%></em>)</span></a>\
                        <a class="commt_a_link" againsted="false" action-type="against" href="javascript:;" title="反对"><span>反对(<em><%=reason%></em>)</span></a>\
                        <a class="commt_a_link" reported="false" action-type="report" href="javascript:;" title="此评论有问题，点击举报"><span>举报</span></a><a class="commt_a_link" href="javascript:;" action-type="reply"><span>回复</span></a>\
                    </span>\
                </div>\
            </dd>\
        </dl>';
cmt_tp = cmt_tp.replace(/([ ]{4,})/g,"");
var quote_tp = '\
        <blockquote id="J_Comment_Quote_<%=ptid%>_<%=tid%>">\
            <div class="commt_info">\
                <p class="title"><span class="re_username"><%=name%></span><span class="re_area">来自[<%=host_name%>]</span><span class="floorCount"><%=quote_num%></span></p>\
                <p class="re_text"><%=comment%></p>\
                <p class="re_mark">\
                    <span action-tid="<%=tid%>">\
                        <a class="commt_a_link" supported="false" action-type="support" href="javascript:;" title="支持"><span>支持(<em><%=score%></em>)</span></a>\
                        <a class="commt_a_link" againsted="false" action-type="against" href="javascript:;" title="反对"><span>反对(<em><%=reason%></em>)</span></a>\
                        <a class="commt_a_link" reported="false" action-type="report" href="javascript:;" title="此评论有问题，点击举报"><span>举报</span></a>\
                        <a class="commt_a_link" href="javascript:;" action-type="reply"><span>回复</span></a>\
                    </span>\
                </p>\
            </div>\
        </blockquote>';
quote_tp = quote_tp.replace(/([ ]{4,})/g,"");
var hot_tp = '\
        <li <%=li_class%>>\
            <div class="comContent">\
                <p class="title">\
                    <span class="userName"><%=name%></span>\
                    <span class="re_area">[<%=host_name%>]</span>\
                    <span class="time"><%=date_show%></span>\
                </p>\
                <div class="con"><em id="hotcon<%=tid%>"><%=comment%></em></div>\
            </div>\
            <div class="re_mark">\
                <span action-tid="<%=tid%>">\
                    <a class="commt_a_link" action-type="share" href="javascript:;" title="分享"><span class="b">分享</span>\
                    <a class="commt_a_link" supported="false" action-type="support" href="javascript:;" title="支持"><span>支持(<em><%=score%></em>)</span></a>\
                    <a class="commt_a_link" againsted="false" action-type="against" href="javascript:;" title="反对"><span>反对(<em><%=reason%></em>)</span></a>\
                    <a class="commt_a_link" reported="false" action-type="report" href="javascript:;" title="此评论有问题，点击举报"><span>举报</span></a>\
                    <a class="commt_a_link" action-type="reply" href="javascript:;"><span>回复</span></a>\
                </span>\
            </div>\
        </li>';
hot_tp = hot_tp.replace(/([ ]{4,})/g,"");
var share_tp = '\
        <div class="share cb_box popshare" id="popshare" data-tid="0">\
            <ul>\
                <li><a class="sicon sina" title="分享到新浪微博" href="javascript:void(0)" data-type="weibosina" onclick="javascript:shareJump(this)" data-from="hotcmt" target="_blank"></a></li>\
                <li><a class="sicon qq" title="分享到QQ空间" href="javascript:void(0)" data-type="qq" onclick="javascript:shareJump(this)" data-from="hotcmt" target="_blank"></a></li>\
                <li><a class="sicon blg" title="分享到搜狐微博" href="javascript:void(0)" data-type="weibosohu" onclick="javascript:shareJump(this)" data-from="hotcmt" target="_blank"></a></li>\
                <li><a class="sicon rrw" title="分享到人人网" href="javascript:void(0)" data-type="renren" onclick="javascript:shareJump(this)" data-from="hotcmt" target="_blank"></a></li>\
                <li><a class="sicon db" title="分享到豆瓣" href="javascript:void(0)" data-type="douban" onclick="javascript:shareJump(this)" data-from="hotcmt" target="_blank"></a></li>\
                <li><a class="sicon itb" title="分享到百度贴吧" href="javascript:void(0)" data-type="itb" onclick="javascript:shareJump(this)" data-from="hotcmt" target="_blank"></a></li>\
            </ul>\
        </div>';
share_tp = share_tp.replace(/([ ]{4,})/g,"");
var emotions_pop_tp = '<div id="J_emotions_pop" style="z-index:11;position:absolute;" class="core_menu pop_show_mini">\
						<div class="core-arrow-top" style="left:0;"><em></em><span></span></div><a href="#" id="J_emotions_close" class="pop_close">关闭</a>\
						<div id="J_emotions_menu"></div>\
						<div class="ct" id="J_emotions_pl"><div class="loading32"></div></div>\
					</div>';
// 分享链接
function shareJump(who,options){
    var settings = {},
        from = $(who).attr('data-from');
    switch(from){
        case 'hotcmt':
            var title = $("TITLE").html().replace('_cnBeta.COM',''),
                tid = $(who).parents("#popshare").attr('data-tid');
            title = title.substr(0,title.lastIndexOf('_'));
            settings = {
                title    : '#自古CB出评论# '+$('#hotcon'+tid).html()+'《RE：'+title+'》',
                shareurl : 'http://www.cnbeta.com/articles/'+GV.DETAIL.SID+'.htm',
                content  : '#自古CB出评论# '+$('#hotcon'+tid).html()+'《RE：'+title+'》'
            };
            break;
        case 'news':
            var parent = $(who).parents("dl").children("dt").find("a");
            settings = {
                title    : parent.html(),
                shareurl : parent.attr("href"),
                content  : parent.html()
            };
            break;
        default: return false;
    }
    if(options) {
        $.extend(settings, options);
    }
    var url = '', e = encodeURIComponent, s = screen, d = document, wd = 0, hg = 0, u = d.location;
    var x = $(who).attr('data-type');
    if(settings.shareurl.indexOf("http://") != 0){
        settings.shareurl = "http://www.cnbeta.com"+settings.shareurl;
    }
    switch (x){
        case 'weibosina':
            url="http://service.t.sina.com.cn/share/share.php?url="+e(settings.shareurl)+"&appkey=695153817&title="+e(settings.title)+"&pic=&ralateUid=&searchPic=false";wd = 615;hg = 505;break;
        case 'weibosohu':
            url="http://t.sohu.com/third/post.jsp?link="+e(settings.shareurl)+"&title="+e(settings.title);wd = 660;hg = 470;break;
        case 'baishehui':
            url="http://bai.sohu.com/share/blank/addbutton.do?link="+e(settings.shareurl)+"&title="+e(settings.title);wd = 480;hg = 340;break;
        case 'douban':
            url ="http://www.douban.com/recommend/?url="+e(settings.shareurl)+"&title="+e(settings.title);wd = 460;hg = 340;break;
        case 'renren':
            url="http://share.renren.com/share/buttonshare?link="+e(settings.shareurl)+"&title="+e(settings.title);wd = 590;hg = 430;break;
        case 'kaixin001':
            url="http://www.kaixin001.com/repaste/share.php?rurl="+e(settings.shareurl)+"&rtitle="+e(settings.title);wd = 540;hg = 360;break;
        case 'itb':
            url="http://tieba.baidu.com/i/sys/share?link="+e(settings.shareurl)+"&type=text&title="+e(settings.title)+"&content="+e(settings.content);wd = 626;hg = 436;break;
        case 'qq':
            url="http://sns.qzone.qq.com/cgi-bin/qzshare/cgi_qzshare_onekey?url="+e(settings.shareurl)+"&desc="+e(settings.content);wd = 900;hg = 500;break;
        case 'tqq':
            url="http://v.t.qq.com/share/share.php?url="+e(settings.shareurl)+'&title='+e(settings.title);wd = 700;hg = 680;break;
        default: return false;
    }
    var a=function(){
        if (!window.open(url, 'cnbetashare', ['toolbar=0,status=0,resizable=1,width='+ wd +',height='+ hg +',left='+ (s.width - wd) / 2+ ',top='+ (s.height - hg) / 2]))
            settings.shareurl = [url].join('');
    }
    if (/Firefox/.test(navigator.userAgent)){
        setTimeout(a, 0);
    }else{
        a();
    }
}
//重新刷新页面，使用location.reload()有可能导致重新提交
function reloadPage(win) {
    var location = win.location;
    location.href = location.pathname + location.search;
}
function scrollToPosition(b){if($("#"+b).length>0){var a=$("#"+b).position().top;$(window).scrollTop(a-50);}return false;}