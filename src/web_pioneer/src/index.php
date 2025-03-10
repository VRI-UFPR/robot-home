<!DOCTYPE html>
<html><head>
    <title> VRI-Pioneer </title>
    <!-- <link href="/bootstrap.min.css" rel="stylesheet"> -->

</head><body>

<div class="container">

    <h1> VRI-Pioneer </h1><hr>
    <?php
        $link = ufr_client("@new zmq:socket @coder msgpack");
        // ufr_put($link, "s\n\n", "odom");
        // $pose = ufr_get($link, "^fff\n");

        /*ufr_put($link, "s\n\n", "scan");
        $scan = ufr_get($link, "^af\n");*/
    ?>

    Base.controle: 
        <button onclick="fetch('/cmd_vel.php?vel=60&rot=0')"> Frente </button>
        <button onclick="fetch('/cmd_vel.php?vel=-60&rot=0')"> Tras </button>
        <button onclick="fetch('/cmd_vel.php?vel=0&rot=5')"> Esquerda </button>
	<button onclick="fetch('/cmd_vel.php?vel=0&rot=-5')"> Direita </button>
	<button onclick="fetch('/cmd_vel.php?vel=0&rot=0')"> Parar </button>
     <br>

    Base: ligada ou desligada?  <br>
    Base.servidor: status <button>ligar</button> <button>desligar</button> <br>
    Base.motores: ligada ou desligada?<br>
    Base.sonares: desligado<br>

	<input type="text" id="text"><button onclick="teste()">OK</button>
    <br>

    lidar: ligado ou desligado?<br>
    camera1: ligado ou desligado<br>
    camera2: ligado ou desligado<br>

    Velocidade: <br>

    Posição: <div id="odom"></div> <br>

    Lidar: <img id="lidar" src="/scan.php" width=320><br>

    Camera: <img id="camera1" src="/camera1.php" width=320> <img id="camera2" src="/camera2.php" width=320>  <br>
    

    ROS: ??Como verificar se o ros estah funcionando bem??<br>

    <h2> Tópicos </h2>
    <?php
        /*$link = ufr_client("@new zmq:socket @coder msgpack @debug 4");
        ufr_put($link, "s\n\n", "scan");
        while (1) {
            $res = ufr_get($link, "^af");
            if ( $res == false ) {
                echo "opa";
                break;
            }
            print_r($res);
            echo "<br><hr>";
        }
        ufr_close($link);*/
    ?>

</div>

<script>

function teste() {
	text = document.getElementById('text');
	fetch('/voice.php?text='+text.value);
}

function update_image() {
    const imagem = document.getElementById('lidar');
    const timestamp = new Date().getTime(); // Adiciona um timestamp para evitar cache
    imagem.src = `scan.php?t=${timestamp}`; // Atualiza a imagem

    const cam1 = document.getElementById('camera1');
    cam1.src = `camera1.php?t=${timestamp}`;

    const cam2 = document.getElementById('camera2');
    cam2.src = `camera2.php?t=${timestamp}`;
}

setInterval(update_image, 500);



function update_odom() {
    fetch('/odom.php').then(response => {
        if (!response.ok) {
            throw new Error('Erro na rede: ' + response.statusText);
        }
        return response.json();
    })
    .then(data => {
        const div = document.getElementById('odom');
        div.innerHTML = `${data}`; // Atualiza o conteúdo da DIV
    })
    .catch(error => {
        console.error('Houve um problema com a requisição:', error);
    });
}
setInterval(update_odom, 500);

</script>

<!-- <script src="bootstrap.bundle.min.js"></script> -->
</body></html>
