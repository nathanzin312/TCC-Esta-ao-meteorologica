
<?php

    $conn= new mysqli("localhost" , "root", "","arduino");
   if($conn-> connect_errno){
      echo "Failed to connect to MySQL: " . $mysqli -> connect_error;
  exit();
   }
   $sql = "SELECT MAX(id) AS id FROM estacao";
  $result = mysqli_query($conn, $sql);
  $row = mysqli_fetch_object($result) ;
  //echo "Maximum Working Hours: " .$row->id;
  $r = $conn -> query("SELECT * FROM estacao WHERE id=($row->id)");
 

?>

<!DOCTYPE html> 
<html> 
<head> 
  <meta charset="UTF-8"> 
  <title>Estação</title> 
  <style>
    * {
      padding: 0;
      margin: 0;
      box-sizing: border-box;
    }
    
    .row {
      display: flex;
      flex-direction: row;
      align-items: center;
      height: 142px;
      border-bottom: 23px solid hsl(130, 54%, 40%);
    }

    .t {
      padding-top: 0px;
      height: 142px;
    }

    .tt {

      height: 142px;
    }

    h1 {
      color: hsl(130, 54%, 40%);
      font-family: sans-serif;
    }

    table {
      padding-top: 80px;
      width: 100%;
      padding-left: 170px;
    }
  </style>
</head> 
<body> 
  <div class="row">
   <div class="position" id="t">
    <img src="logo.png">
  </div>
  <div class="position" id="tt">
    <h1>Estação Meteorológica Campus Avançado Goioerê</h1>
  </div>
</div>
<div class="row-2">
  <table  border-collapse> 
    <tr> <?php $dado = $r->fetch_array() ?>
      <td width="100" height="40">Temperatura:<?php echo $dado['temperatura'];?> C</td>
      <td width="100" height="40">Pressão:<?php echo $dado['pressao'];?> hPa</td>
      <td width="100" height="40">Umidade: <?php echo $dado['umidade'];?>%</td> 
      <td width="100" height="40">Velocidade Do Vento:<?php echo $dado['velocidade'];?> K/h</td> 
      <td width="100" height="40">Direçao:<?php echo $dado['direcao'];?>Graus</td> 
    </tr>
    </tr> 
  </table>
</div> 
</body> 
</html>     
 
