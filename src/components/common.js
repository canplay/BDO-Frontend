export default {
  uuid() {
    return "xxxxxxxx-xxxx-yxxx-yxxx-xxxxxxxxxxxx".replace(/[xy]/g, function(c) {
      let r = (Math.random() * 16) | 0,
        v = c === "x" ? r : (r & 0x3) | 0x8;
      return v.toString(16);
    });
  },

  GetUrlParamValue(name) {
    let regex = window.location.search
      .substr(1)
      .match("(^|&)" + name + "=([^&]*)(&|$)", "i");
    if (!regex) return null;
    return decodeURIComponent(regex[2]);
  },

  GetRequest() {
    let url = location.search;
    let theRequest = new Object();
    if (url.indexOf("?") != -1) {
      let str = url.substr(1);
      let strs = str.split("&");
      strs.forEach(item => {
        theRequest[item.split("=")[0]] = decodeURIComponent(item.split("=")[1]);
      });
    }
    return theRequest;
  },

  RequestURL(url, data, type, callback) {
    let xhr = new XMLHttpRequest();

    if (type === "GET") {
      xhr.open("GET", url + data === "" ? "" : "?" + data, true);
      xhr.send();
    }
    else {
      xhr.open("POST", url, true);
      xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
      xhr.send(data);
    }

    xhr.onreadystatechange = function () {
      if (xhr.readyState == 4 && xhr.status == 200) {
        callback("success", xhr.responseText);
      }
      else { 
        callback("failed", xhr.status);
      }
    };
  },

  _endWith(string, str) {
    if (arguments.length != 2) {
      throw "arg number error";
    }
    return string.slice(-str.length) == str;
  },

  _formatString() {
    if (arguments.length != 1) {
      throw "arg number error";
    }
    var reg = /([a-zA-Z])/g;
    var regarray = arguments[0].match(reg);
    if (regarray == null) {
      return arguments[0].split(".");
    }
    if (regarray && regarray.length != 1) {
      throw "version format error";
    }
    var regString = regarray.join("");
    if (this._endWith(arguments[0], regString) != true) {
      throw "version format error";
    }
    return arguments[0]
      .replace(regString, "." + regString.charCodeAt())
      .split(".");
  },

  Compare() {
    if (arguments.length > 2) {
      throw "arg number error";
    } else if (arguments.length < 2) {
      throw "arg number error";
    } else {
      if (arguments[0] == arguments[1]) {
        return 0;
      }
      var arr1 = this._formatString(arguments[0]);
      var arr2 = this._formatString(arguments[1]);
      var length = Math.min(arr1.length, arr2.length);
      for (var i = 0; i != length; i++) {
        if (arr1[i] > arr2[i]) {
          return 1;
        } else if (arr1[i] < arr2[i]) {
          return -1;
        }
      }
      if (length == arr1.length) {
        return -1;
      } else {
        return 1;
      }
    }
  }
};
