<?php

$val = $_GET["data"];

$timezone = date('m/d/Y h:i:s a', time());

$fileContent="The current server timezone is: " . $timezone."\n".$val."\n";

$fileStatus=file_put_contents("datastorage.txt",$fileContent,FILE_APPEND);

if($fileStatus != false)

{

                echo  "SUCCESS. Data written in file". "\n" . "The current server timezone is: " . $timezone;

  $handle = @fopen("datastorage.txt", "r");
if ($handle) {
    while (($content = fgets($handle, 4096)) !== false) {
      echo "datastorage.txt file content is: \n";   
      echo $content." \n";
    }
    if (!feof($handle)) {
      echo "Can not open datastorage.txt file \n";  
      echo "Error: unexpected fgets() fail\n";
    }
    fclose($handle);
}
  
}

else

{

                echo  "FAIL. Could not connect to file." . "\n" . "The current server timezone is: " . $timezone;

}

?>
