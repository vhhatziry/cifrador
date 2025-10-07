<?php

function mostrarError($mensaje) {
    echo <<<HTML
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <title>Error en el Proceso</title>
    <link rel="stylesheet" href="estilos.css">
    <style>
        .error-mensaje {
            background-color: #5c3b3b;
            color: #ffcccc;
            padding: 15px;
            border-radius: 8px;
            border: 1px solid #c87a7a;
            margin-top: 20px;
            text-align: left;
        }
        a.boton-volver {
            display: inline-block;
            margin-top: 20px;
            padding: 10px 20px;
            background-color: var(--acento-vibrante);
            color: white;
            text-decoration: none;
            border-radius: 5px;
            transition: background-color 0.3s;
        }
        a.boton-volver:hover {
            background-color: var(--acento-hover);
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Ha Ocurrido un Error</h1>
        <div class="error-mensaje">
            <p><strong>Detalles del error:</strong></p>
            <p>$mensaje</p>
        </div>
        <a href="index.html" class="boton-volver">Volver a Intentar</a>
    </div>
</body>
</html>
HTML;
}



$uploadDir = 'uploads/';
if (!is_dir($uploadDir)) {
    mkdir($uploadDir, 0777, true);
}

// 1. Recoger los datos del formulario
$alpha = escapeshellarg($_POST['alpha']);
$beta = escapeshellarg($_POST['beta']);
$opcion = escapeshellarg($_POST['opcion']);

// 2. Manejar el archivo subido
if (isset($_FILES['archivo']) && $_FILES['archivo']['error'] == 0) {
    $nombreTemporalEntrada = $uploadDir . uniqid() . '_' . basename($_FILES['archivo']['name']);

    if (move_uploaded_file($_FILES['archivo']['tmp_name'], $nombreTemporalEntrada)) {

        // 3. Construir y ejecutar el comando
        $comando = "afin_cipher.exe " . $alpha . " " . $beta . " " . $opcion . " " . $nombreTemporalEntrada;
        $nombreArchivoSalida = shell_exec($comando);
        $nombreArchivoSalida = trim($nombreArchivoSalida);

        // 4. Verificar si el archivo de salida se generó
        if (!empty($nombreArchivoSalida) && file_exists($nombreArchivoSalida)) {
            // Forzar la descarga del archivo generado
            header('Content-Description: File Transfer');
            header('Content-Type: application/octet-stream');
            header('Content-Disposition: attachment; filename="' . basename($nombreArchivoSalida) . '"');
            header('Expires: 0');
            header('Cache-Control: must-revalidate');
            header('Pragma: public');
            header('Content-Length: ' . filesize($nombreArchivoSalida));
            readfile($nombreArchivoSalida);

            // 5. Limpieza
            unlink($nombreTemporalEntrada);
            unlink($nombreArchivoSalida);
            exit;

        } else {
            // LLAMAMOS A LA NUEVA FUNCIÓN DE ERROR
            $error_info = "El programa en C no generó un archivo de salida.<br><br>";
            $error_info .= "<b>Comando ejecutado:</b> " . htmlspecialchars($comando) . "<br>";
            $error_info .= "<b>Respuesta del programa:</b> " . htmlspecialchars($nombreArchivoSalida);
            mostrarError($error_info);
        }

    } else {
        mostrarError("No se pudo mover el archivo subido al servidor. Verifique los permisos de la carpeta.");
    }
} else {
    // Manejo de errores de subida más específico
    $codigoError = $_FILES['archivo']['error'] ?? 'No especificado';
    mostrarError("Hubo un problema al subir el archivo. Código de error de PHP: " . $codigoError);
}

?>