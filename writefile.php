 <?php

$val = $_GET["data"];

$timezone = date('m/d/Y h:i:s a', time());

$fileContent="The current server timezone is: " . $timezone."\n".$val."\n";
$path=getcwd()."/datastorage.txt";

//if(!file_exists("Data_Dir")) {
//        mkdir($_SERVER['DOCUMENT_ROOT']."Data_Dir/" . $this->getUser() . "/" , "0777", true);     
// }
//if(!file_exists("Data_Dir")) {
//   echo "Can not Make Data_Dir folder because of permission \n";
//}
chmod($path, 0777); 
if( !chmod($path, 0777) ) {
    // more code
   echo "Can not chang datastorage.txt file chmod permission \n";
   echo getcwd();// will return the current file name only

    $file=getcwd()."/datastorage1".".txt";
    $fp = fopen($file, 'w');
    fwrite($fp, $content);
    fclose($fp);

    // Set perms with chmod()
    chmod($file, 0777);
     echo "Content saved in "."datastorage1".".txt file \n";

  
}

$fileStatus=file_put_contents($path,$fileContent,FILE_APPEND);

if($fileStatus != false)

{

                echo  "SUCCESS. Data written in file". "\n" . "The current server timezone is: " . $timezone;

  $handle = @fopen($path, "r");
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
