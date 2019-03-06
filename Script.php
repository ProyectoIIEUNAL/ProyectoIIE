<?php


require_once './vendor/autoload.php';

use Kreait\Firebase\Factory;
use Kreait\Firebase\ServiceAccount;


class Select
{

protected $database;
protected $dbname ='Datos';

    public function __construct()
    {
        $acc = ServiceAccount::fromJsonFile(_DIR_. '/secret/proyectoiieunal-bdba809db527.json');
        $firebase = (new Factory)->withServiceAccount($acc)->create();

        $this->database = $firebase->getDatabase();
    }

    public function get(int $selectID = Null)
    {
        if(empty($selectID) || !isset($selectID)){return False;}
        if($this->database->getReference($this->dbname)->getSnapshot()->hasChild($selectID)){
            return $this->database->getReference($this->dbname)->getChild($selectID)->getValue();
        }
    }

    public function insert(array $data, $distancia)
    {
        if(empty($data) || !isset($data)){return False;}

            $this->database->getReference()->getChild($this->dbname)->push($data);
        $this->database->getReference()->getChild("NivelAgua")->getChild('distancia')->set($_POST['d']);

        return TRUE;

    }

    public function delete(int $selectID)
    {
        if(empty($selectID) || !isset($selectID)){return False;}

        if($this->database->getReference($this->dbname)->getSnapshot()->hasChild($selectID))
        {
            $this->database->getReference($this->dbname)->getChild($selectID)->remove();
            return TRUE;
        }
        else{return FALSE;}
    }

}


$selects = new Select();

$date = getdate();
$horas = 0;

if((int)$date['hours']<5)
{
    $horas = (int)($date['hours'])+19;
}
else
{
    $horas = (int)($date['hours'])-5;
}

var_dump($selects->insert([
    'Fecha'=> $date['mday'].'/'.$date['mon'].'/'.$date['year'].'-'.(string)($horas).':'.$date['minutes'],
    'Temperatura'=>$_POST['T'],
    'Humedad'=>$_POST['H'],
    'HumedadT'=>$_POST['HT']
],$_POST['d']));
