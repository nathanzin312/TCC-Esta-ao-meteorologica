
<?php

    $conn= new mysqli("localhost" , "root", "","arduino");
   if($conn-> connect_errno){
      echo "Failed to connect to MySQL: " . $mysqli -> connect_error;
  exit();
   }
    
   $temperatura = $_GET['temperatura'];
   $pressao = $_GET['pressao'];
   $umidade = $_GET['umidade'];
   $velocidade = $_GET['velocidade'];
   $direcao = $_GET['direcao']; 

   if($temperatura == null || $umidade == null|| $pressao == null||  $direcao == null){
      echo "valor nullo";
   }
    else{
    $sql = "INSERT INTO estacao (temperatura,pressao,umidade,velocidade,direcao)
      VALUES ('$temperatura', '$pressao', '$umidade', '$velocidade','$direcao')";

   if ($conn->query($sql) === TRUE) {
    session_start();
    $last_id =  $conn->insert_id;
    $_SESSION['id'] = $last_id;
   echo "New record created successfully" .$_SESSION['id'] ;
} else {
  echo "Error: " . $sql . "<br>" . $conn->error;
}   
}
header("Location: arduino.php");
?>