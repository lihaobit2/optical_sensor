// $Id: fancy_login.js,v 1.19 2011/01/26 15:31:23 hakulicious Exp $
(function($)
{
	var popupVisible = false;
	var ctrlPressed = false;

	function showLogin()
	{
		var settings = Drupal.settings.fancyLogin;
		var loginBox = $("#fancy_login_login_box");
		if(!popupVisible) {
			popupVisible = true;
			if(settings.hideObjects) {
				$("object, embed").css("visibility", "hidden");
			}
			$("#fancy_login_dim_screen").css({"position" : "fixed", "top" : "0", "left" : "0", "height" : "100%", "width" : "100%", "display" : "block", "background-color" : settings.screenFadeColor, "z-index" : settings.screenFadeZIndex, "opacity" : "0"}).fadeTo(settings.dimFadeSpeed, 0.8, function()
			{
				loginBox.css({"position" : "fixed", "width" : settings.loginBoxWidth, "height" : settings.loginBoxHeight});
				var wHeight = window.innerHeight ? window.innerHeight : $(window).height();
				var wWidth = $(window).width();
				var eHeight = loginBox.height();
				var eWidth = loginBox.width();
				var eTop = (wHeight - eHeight) / 2;
				var eLeft = (wWidth - eWidth) / 2;
				if($("#fancy_login_close_button").css("display") === "none") {
					$("#fancy_login_close_button").css("display", "inline");
				}
				loginBox.css({"top" : eTop, "left" : eLeft, "color" : settings.loginBoxTextColor, "background-color" : settings.loginBoxBackgroundColor, "border-style" : settings.loginBoxBorderStyle, "border-color" : settings.loginBoxBorderColor, "border-width" : settings.loginBoxBorderWidth, "z-index" : (settings.screenFadeZIndex + 1), "display" : "none", "padding-left" : "15px", "padding-right" : "15px"})
				.fadeIn(settings.boxFadeSpeed);
				loginBox.find(".form-text:first").focus().select();
				setCloseListener();
			});
		}
	}

	function setCloseListener()
	{
		$("#fancy_login_dim_screen, #fancy_login_close_button").click(function()
		{
			hideLogin();
			return false;
		});
		$("#fancy_login_login_box form").submit(function()
		{
			submitted();
		});
		$("#fancy_login_login_box a:not('#fancy_login_close_button')").click(function()
		{
			submitted();
		});
		$(document).keyup(function(event)
		{
		    if(event.keyCode === 27) {
		        hideLogin();
		    }
		});
	}

	function hideLogin()
	{
		var settings = Drupal.settings.fancyLogin;
		if(popupVisible) {
			popupVisible = false;
			$("#fancy_login_login_box").fadeOut(settings.boxFadeSpeed, function()
			{
				$(this).css({"position" : "static", "height" : "auto", "width" : "auto",  "background-color" : "transparent", "border" : "none" });
				$("#fancy_login_dim_screen").fadeOut(settings.dimFadeSpeed, function()
				{
					if(settings.hideObjects) {
						$("object, embed").css("visibility", "visible");
					}
				});
				$(window).focus();
			});
		}
	}

	function submitted(requestPassword)
	{
		var formContents = $("#fancy_login_form_contents");
		var ajaxLoader = $("#fancy_login_ajax_loader");
		var wHeight = formContents.height();
		var wWidth = formContents.width();
		ajaxLoader.css({"height" : wHeight, "width" : wWidth});
		formContents.fadeOut(300, function()
		{
			ajaxLoader.fadeIn(300);
			var img = ajaxLoader.children("img:first");
			var imgHeight = img.height();
			var imgWidth = img.width();
			var eMarginTop = (wHeight - imgHeight) / 2;
			var eMarginLeft = (wWidth - imgWidth) / 2;
			img.css({"margin-left" : eMarginLeft, "margin-top" : eMarginTop});
			if(requestPassword) {
				getRequestPassword();
			}
		});
	}

	function getRequestPassword()
	{
		var settings = Drupal.settings;
		var passwordPath = settings.fancyLogin.loginPath.replace(/login/, "password");
		$.ajax(
		{
			url:settings.basePath + passwordPath,
			dataFilter:function(data)
			{
				return $(data).find("#user-pass");
			},
			success:function(data)
			{
				var formContents = $("#fancy_login_form_contents");
				formContents.children("form").css("display", "none");
				var itemList = formContents.find(".item-list");
				itemList.before(data);
				$("#fancy_login_ajax_loader").fadeOut(300, function()
				{
					toggle = $("<li><a id=\"toggle_link\" href=\"#\">" + Drupal.t("Login") + "</a></li>");
					toggle.click(function()
					{
						toggleForm();
					});
					itemList.children("ul").append(toggle);
					$("#user-pass").attr("action", $("#user-pass").attr("action") + settings.fancyLogin.requestDestination);
					formContents.fadeIn(300);
				});
			}
		});
	}

	function toggleForm()
	{
		currentForm = ($("#fancy_login_form_contents #user-login").css("display") === "none") ? "#user-pass" : "#user-login";
		targetForm = (currentForm === "#user-login") ? "#user-pass" : "#user-login";
		linkText = (currentForm === "#user-login") ? Drupal.t("Login") : Drupal.t("Request new password");
		$(currentForm).fadeOut(300, function()
		{
			$(targetForm).fadeIn(300);
		});
		$("#fancy_login_form_contents .item-list").fadeOut(300, function()
		{
			$("#toggle_link").text(linkText);
			$("#fancy_login_form_contents .item-list").fadeIn(300);
		});
	}

	Drupal.behaviors.fancyLogin = 
	{
		attach:function()
		{
			var settings = Drupal.settings.fancyLogin;
			if(!$.browser.msie || $.browser.version > 6 || window.XMLHttpRequest) {
				$("a[href*='" + settings.loginPath + "']").each(function()
				{
					if(settings.destination) {
						var targetHREF = $(this).attr("href");
						if(targetHREF.search(/destination=/i) === -1) {
							targetHREF += settings.destination;
							$(this).attr("href", targetHREF);
						}
					}
					$(this).click(function()
					{
						var action = $(this).attr("href");
						if(settings.httpsRoot) {
							action = settings.httpsRoot + action;
						}
						$("#fancy_login_login_box form").attr("action", action);
						showLogin();
						return false;
					});
				});
				$(document).keyup(function(e)
				{
					if(e.keyCode === 17) {
						ctrlPressed = false;
					}
				});
				$(document).keydown(function(e)
				{
					if(e.keyCode === 17) {
						ctrlPressed = true;
					}
					if(ctrlPressed === true && e.keyCode === 190) {
						ctrlPressed = false;
						if(popupVisible) {
							hideLogin();
						} else {
							showLogin();
						}
					}
				});
				$("#fancy_login_login_box a[href*='" + settings.loginPath.replace(/login/, "password") + "']").click(function()
				{
					$(this).parent().fadeOut(200);
					submitted(true);
					return false;
				});
			}
		}
	};
}(jQuery));;
(function(e){Drupal.behaviors.image_caption={attach:function(t,n){e("img.caption:not(.caption-processed)").each(function(t){var n=e(this).width()?e(this).width():false;var r=e(this).height()?e(this).height():false;var i=e(this).attr("title");var s=e(this).attr("alt");if(e(this).attr("align")){var o=e(this).attr("align");e(this).css({"float":o});e(this).removeAttr("align")}else if(e(this).css("float")){var o=e(this).css("float")}else{var o="normal"}var u=e(this).attr("style")?e(this).attr("style"):"";e(this).removeAttr("width");e(this).removeAttr("height");e(this).css("width","");e(this).css("height","");e(this).removeAttr("align");e(this).removeAttr("style");e(this).wrap('<span class="image-caption-container" style="display:inline-block;'+u+'"></span>');e(this).parent().addClass("image-caption-container-"+o);if(n){e(this).width(n);e(this).parent().width(n)}if(r){e(this).height(r)}if(s.length>0){e(this).parent().append('<span style="display:block;" class="image-caption">'+'<span style="display:block;" class="image-alt-caption">'+s+"</span>"+"</span>")}else if(i.length>0){e(this).parent().append('<span style="display:block;" class="image-caption">'+'<span style="display:block;" class="image-title-caption">'+i+"</span>"+"</span>")}e(this).addClass("caption-processed")})}}})(jQuery);

(function ($) {
  Drupal.Panels = {};

  Drupal.Panels.autoAttach = function() {
    if ($.browser.msie) {
      // If IE, attach a hover event so we can see our admin links.
      $("div.panel-pane").hover(
        function() {
          $('div.panel-hide', this).addClass("panel-hide-hover"); return true;
        },
        function() {
          $('div.panel-hide', this).removeClass("panel-hide-hover"); return true;
        }
      );
      $("div.admin-links").hover(
        function() {
          $(this).addClass("admin-links-hover"); return true;
        },
        function(){
          $(this).removeClass("admin-links-hover"); return true;
        }
      );
    }
  };

  $(Drupal.Panels.autoAttach);
})(jQuery);
;
/**
 * @file
 * JavaScript for the Disqus Drupal module.
 */

// The Disqus global variables.
var disqus_shortname = '';
var disqus_url = '';
var disqus_title = '';
var disqus_identifier = '';
var disqus_developer = 0;
var disqus_def_name = '';
var disqus_def_email = '';
var disqus_config;

(function ($) {

/**
 * Drupal Disqus behavior.
 */
Drupal.behaviors.disqus = {
  attach: function (context, settings) {
    $('body').once('disqus', function() {
      // Load the Disqus comments.
      if (settings.disqus || false) {
        // Setup the global JavaScript variables for Disqus.
        disqus_shortname = settings.disqus.domain;
        disqus_url = settings.disqus.url;
        disqus_title = settings.disqus.title;
        disqus_identifier = settings.disqus.identifier;
        disqus_developer = settings.disqus.developer || 0;
        disqus_def_name = settings.disqus.name || '';
        disqus_def_email = settings.disqus.email || '';

        // Language and SSO settings are passed in through disqus_config().
        disqus_config = function() {
          if (settings.disqus.language || false) {
            this.language = settings.disqus.language;
          }
          if (settings.disqus.remote_auth_s3 || false) {
            this.page.remote_auth_s3 = settings.disqus.remote_auth_s3;
          }
          if (settings.disqus.api_key || false) {
            this.page.api_key = settings.disqus.api_key;
          }
          if (settings.disqus.sso || false) {
            this.sso = settings.disqus.sso;
          }
          this.callbacks.onNewComment = [ function()
        	{
        	  s.linkTrackVars="events,s.prop";
        	  s.linkTrackEvents="event9";
        	  s.events="event9"
        	  s=s_gi(s_account); s.tl(this, 'o', 'Comment on article');
        	  console.log(s);
        	}]
        };

        // Make the AJAX call to get the Disqus comments.
        jQuery.ajax({
            type: 'GET',
            url: 'http://' + disqus_shortname + '.disqus.com/embed.js',
            dataType: 'script',
            cache: false
          });
      }

      // Load the comment numbers JavaScript.
      if (settings.disqusComments || false) {
        disqus_shortname = settings.disqusComments;
        // Make the AJAX call to get the number of comments.
        jQuery.ajax({
          type: 'GET',
          url: 'http://' + disqus_shortname + '.disqus.com/count.js',
          dataType: 'script',
          cache: false
        });
      }
    });
  }
};

})(jQuery);
;
(function ($) {

Drupal.googleanalytics = {};

$(document).ready(function() {

  // Attach mousedown, keyup, touchstart events to document only and catch
  // clicks on all elements.
  $(document.body).bind("mousedown keyup touchstart", function(event) {

    // Catch the closest surrounding link of a clicked element.
    $(event.target).closest("a,area").each(function() {

      // Is the clicked URL internal?
      if (Drupal.googleanalytics.isInternal(this.href)) {
        // Skip 'click' tracking, if custom tracking events are bound.
        if ($(this).is('.colorbox')) {
          // Do nothing here. The custom event will handle all tracking.
          //console.debug("Detected click on colorbox.");
        }
        // Is download tracking activated and the file extension configured for download tracking?
        else if (Drupal.settings.googleanalytics.trackDownload && Drupal.googleanalytics.isDownload(this.href)) {
          // Download link clicked.
          ga("send", "event", "Downloads", Drupal.googleanalytics.getDownloadExtension(this.href).toUpperCase(), Drupal.googleanalytics.getPageUrl(this.href));
        }
        else if (Drupal.googleanalytics.isInternalSpecial(this.href)) {
          // Keep the internal URL for Google Analytics website overlay intact.
          ga("send", "pageview", { page: Drupal.googleanalytics.getPageUrl(this.href) });
        }
      }
      else {
        if (Drupal.settings.googleanalytics.trackMailto && $(this).is("a[href^='mailto:'],area[href^='mailto:']")) {
          // Mailto link clicked.
          ga("send", "event", "Mails", "Click", this.href.substring(7));
        }
        else if (Drupal.settings.googleanalytics.trackOutbound && this.href.match(/^\w+:\/\//i)) {
          if (Drupal.settings.googleanalytics.trackDomainMode == 2 && Drupal.googleanalytics.isCrossDomain(this.hostname, Drupal.settings.googleanalytics.trackCrossDomains)) {
            // Top-level cross domain clicked. document.location is handled by _link internally.
            event.preventDefault();
            //console.debug("Detected click to cross domain url '%s'.", this.href);
            // @todo: unknown upgrade path
            //_gaq.push(["_link", this.href]);
            //ga("link", this.href); ???
          }
          else {
            // External link clicked.
            ga("send", "event", "Outbound links", "Click", this.href);
          }
        }
      }
    });
  });

  // Colorbox: This event triggers when the transition has completed and the
  // newly loaded content has been revealed.
  $(document).bind("cbox_complete", function () {
    var href = $.colorbox.element().attr("href");
    if (href) {
      ga("send", "pageview", { page: Drupal.googleanalytics.getPageUrl(href) });
    }
  });

});

/**
 * Check whether the hostname is part of the cross domains or not.
 *
 * @param string hostname
 *   The hostname of the clicked URL.
 * @param array crossDomains
 *   All cross domain hostnames as JS array.
 *
 * @return boolean
 */
Drupal.googleanalytics.isCrossDomain = function (hostname, crossDomains) {
  /**
   * jQuery < 1.6.3 bug: $.inArray crushes IE6 and Chrome if second argument is
   * `null` or `undefined`, http://bugs.jquery.com/ticket/10076,
   * https://github.com/jquery/jquery/commit/a839af034db2bd934e4d4fa6758a3fed8de74174
   *
   * @todo: Remove/Refactor in D8
   */
  if (!crossDomains) {
    return false;
  }
  else {
    return $.inArray(hostname, crossDomains) > -1 ? true : false;
  }
}

/**
 * Check whether this is a download URL or not.
 *
 * @param string url
 *   The web url to check.
 *
 * @return boolean
 */
Drupal.googleanalytics.isDownload = function (url) {
  var isDownload = new RegExp("\\.(" + Drupal.settings.googleanalytics.trackDownloadExtensions + ")$", "i");
  return isDownload.test(url);
}

/**
 * Check whether this is an absolute internal URL or not.
 *
 * @param string url
 *   The web url to check.
 *
 * @return boolean
 */
Drupal.googleanalytics.isInternal = function (url) {
  var isInternal = new RegExp("^(https?):\/\/" + window.location.host, "i");
  return isInternal.test(url);
}

/**
 * Check whether this is a special URL or not.
 *
 * URL types:
 *  - gotwo.module /go/* links.
 *
 * @param string url
 *   The web url to check.
 *
 * @return boolean
 */
Drupal.googleanalytics.isInternalSpecial = function (url) {
  var isInternalSpecial = new RegExp("(\/go\/.*)$", "i");
  return isInternalSpecial.test(url);
}

/**
 * Extract the relative internal URL from an absolute internal URL.
 *
 * Examples:
 * - http://mydomain.com/node/1 -> /node/1
 * - http://example.com/foo/bar -> http://example.com/foo/bar
 *
 * @param string url
 *   The web url to check.
 *
 * @return string
 *   Internal website URL
 */
Drupal.googleanalytics.getPageUrl = function (url) {
  var extractInternalUrl = new RegExp("^(https?):\/\/" + window.location.host, "i");
  return url.replace(extractInternalUrl, '');
}

/**
 * Extract the download file extension from the URL.
 *
 * @param string url
 *   The web url to check.
 *
 * @return string
 *   The file extension of the passed url. e.g. "zip", "txt"
 */
Drupal.googleanalytics.getDownloadExtension = function (url) {
  var extractDownloadextension = new RegExp("\\.(" + Drupal.settings.googleanalytics.trackDownloadExtensions + ")$", "i");
  var extension = extractDownloadextension.exec(url);
  return (extension === null) ? '' : extension[1];
}

})(jQuery);
;
