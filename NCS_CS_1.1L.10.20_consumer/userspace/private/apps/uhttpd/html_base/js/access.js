//4;root; 2:admin;  1:normal user
var user_level = "<? echo(user_level)?>";
//var user_level = 2;
var hascheckUserHideSSID = 0;

var dom_access = [
  {"id": "left_basicsettings", "attr": 4},
  {"id": "left_advanced", "attr": 4},
  {"id": "left_password", "attr": 2},
  {"id": "left_upgrade", "attr": 4},
  {"id": "left_restore", "attr": 6},
  {"id": "left_tr69", "attr": 0},
  {"id": "left_localssh", "attr": 4},
  {"id": "top_chanpass_li", "attr": 2},
];

if (document.all){
  window.attachEvent('onload',init_access)// IE
}
else{
  window.addEventListener('load',init_access,false);//firefox
}

function init_access(){
  user_level = (user_level == "") ? 1 : user_level;
  var i_userlevel = parseInt(user_level);

  for (key in dom_access){
    var _id = dom_access[key].id;
    var _attr = dom_access[key].attr;
    //console.log("id = " + _id + ", attr = " + _attr);
    
    if(document.getElementById(_id) == null)    continue;

    if (!(_attr & i_userlevel)){
        document.getElementById(_id).style.display = "none";
    }
  }
}

//admin(2): only show primarry ssid
//return 0: not need hide second ssid.    1:need hide second ssid
function checkUserHideSSID(index_ssid){
  if (hascheckUserHideSSID == 0)  return 0;

  var i_userlevel = parseInt(user_level);
  if (i_userlevel == 2 && !(index_ssid == 1 || index_ssid == 5)){
      return 1;
  }
  
  return 0;
}
