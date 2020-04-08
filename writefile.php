<?php

$val = "data";
$timezone = date('m/d/Y h:i:s a', time());

$fileContent="The current server timezone is: " . $timezone."\n".$val."\n";

$fileStatus=file_put_contents("datastorage.txt",$fileContent,FILE_APPEND);

if($fileStatus != false)

{

                echo  "SUCCESS. Data written in file". "\n" . "The current server timezone is: " . $timezone;

}

else

{

                echo  "FAIL. Could not connect to file." . "\n" . "The current server timezone is: " . $timezone;

}

?>
