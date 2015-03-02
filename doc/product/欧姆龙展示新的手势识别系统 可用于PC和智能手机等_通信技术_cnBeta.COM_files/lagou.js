/* cnBeta.COM job channel */
(function($) {
    $.extend({
        //过滤非法字符
		stripscript : function(s){
			var pattern = new RegExp("[`~!@#$^&*()=|{}':;',\\[\\].<>/?~！@#￥……&*（）——|{}【】‘；：”“'。，、？%]");
			var rs = "";
			for (var i = 0; i < s.length; i++) {
				rs = rs+s.substr(i, 1).replace(pattern, '');
			}
		    return rs;
		},
        initJob : function(options){
            var op = {
                url            : 'http://job.cnbeta.com/jobs/list_',
                param        : {'kd':'','pl':'','gj':'','xl':'','yx':'','gx':'','st':'','city':'','types':'cnbeta','keywords':''},
                template    : '<dl><dt><a href="<%=posiitonDetailUrl%>" target="_blank"><%=positionName%></a><span class="ml10 c9">[<%=city%>]</span></dt><dd><span>月薪： <em class="c7"><%=salary%></em></span><br /><span>公司： <em class="c7"><a href="<%=comUrl%>" target="_blank"><%=companyName%></a></em></span><br /><span>职位诱惑： <em class="c7"><%=positionAdvantage%></em></span></dd></dl>',
                container    : "#job_box"
            };
            if(options)
                $.extend(op, options);
            var box = $(op.container),items = '',key = $("#news_title").text() || $("meta[name=keywords]").attr('content');
            if(box.length < 1)
                return false;
            //op.url = op.url.replace('{key}',$.stripscript(key));
            op.param.keywords = $.stripscript(key);

            $.ajax({
                async:true,
                timeout:2000,
                type:"get",
                url:op.url,
                data:op.param,
                dataType:"jsonp",
                jsonpCallback:'success_jsonpCallback',
                success:function(data, status){
                    for(var el in data){
                        if(el > 5) break;
                        if(data[el]){
                            items += CB.tmpl(op.template, data[el]);
                        }
                    }
                    if(items){
                        box.html(items).parent().fadeIn(1000, function() {
                            if($("#fixed_area").length > 0 && fixed_top != 'undefined'){
                                fixed_top = $("#fixed_area").offset().top;
                            }
                        });
                    }
                }
            });
        }
    });
    $.initJob();
})(jQuery);