$(document).ready(function() {
  
    $.fn.myFunction = function(buttonlable){ 
var b= '#'+ buttonlable;

        var t =$('#focusedInput').val();
   var cs =$('#csInput').val();
   if (cs!="" && cs!=null)
     {
         var api2 = "http://"+cs+":8080/"+t+"/pin/"+buttonlable;
     }else
       {
           var api2 = "http://blynk-cloud.com:8080/"+t+"/pin/"+buttonlable;
       }
 


    $.getJSON(api2,function(data3){      
      var obj = JSON.parse(data3)
      
     
if(obj==1){
   document.getElementById(buttonlable).value="Off";

  $(b).attr('checked', true);

}

});

    }
  
  
 $("#sub").click(function(){
   
   var t =$('#focusedInput').val();
   var cs =$('#csInput').val();
   if (cs!="" && cs!=null)
     {
         var api = "http://"+cs+":8080/"+t+"/project";
        var api3 = "http://"+cs+":8080/"+t+"/isHardwareConnected";
     }else
       {
           var api = "http://blynk-cloud.com:8080/"+t+"/project";
             var api3 = "http://blynk-cloud.com:8080/"+t+"/isHardwareConnected";
       }
 
   

    $.getJSON(api,function(data){      
var projectname =data.name;
 var boardTyp = data.boardType;
  $("#pn").html(projectname);
    $("#bt").html(boardTyp);
});

        $.getJSON(api3,function(data4){      


 if(data4==true){
           var hs =  "<span class='label label-success'>ONLINE</span>";
        }else{
            var hs =  "<span class='label label-danger'>OFFLINE</span>";
        }

    $("#hs").html(hs);
});
 });


   $("#subpin").click(function () {
     
var buttonlable = $('#epin').val();
     var divid = "div"+buttonlable;
     if(buttonlable!="")
     var bt ="<div id="+divid+" class='alert'><div class='col-sm-2'><legend>"+buttonlable+"</legend></div><div class='col-sm-2'><label class='switch'><input type='checkbox' id="+buttonlable+" value='On'   onclick='toggleCheckbox(this)' ><div class='slider round'></div></label></div><div class='col-sm-2'><a href='#' class='btn btn-danger' id="+divid+"    onclick='remo(this)'>Remove</a></div></div><br>";
     
 $("#adddata").append(bt); 
     $('#epin').val("");
      $.fn.myFunction(buttonlable);
    });
  
  
  
  
  
   $("#slidepin").click(function () {
     
var buttonlable = $('#epin').val();
     var divid = "div"+buttonlable;
     if(buttonlable!="")
     var bt ="<div id="+divid+" class='alert'><div class='col-sm-2'><legend>"+buttonlable+"</legend></div><div class='col-sm-2'><input id="+buttonlable+" type='range' min='0' max='100' value='0' onchange='ranchang(this)'  /></div><div class='col-sm-2'><a href='#' class='btn btn-danger' id="+divid+"    onclick='remo(this)'>Remove</a></div></div><br>";
     
 $("#adddata").append(bt); 
     $('#epin').val("");
      $.fn.myFunction(buttonlable);
    });
  

});




