<?php
// Directorio temporal para subir y generar archivos.
$uploadDir = 'uploads/';
if (!is_dir($uploadDir)) {
    mkdir($uploadDir, 0777, true);
}

// 1. Recoger los datos del formulario
$alpha = escapeshellarg($_POST['alpha']); // escapeshellarg() es por seguridad
$beta = escapeshellarg($_POST['beta']);
$opcion = escapeshellarg($_POST['opcion']);

// 2. Manejar el archivo subido
if (isset($_FILES['archivo']) && $_FILES['archivo']['error'] == 0) {
    // Crear un nombre de archivo único para evitar conflictos
    $nombreTemporalEntrada = $uploadDir . uniqid() . '_' . basename($_FILES['archivo']['name']);

    // Mover el archivo subido a nuestro directorio
    if (move_uploaded_file($_FILES['archivo']['tmp_name'], $nombreTemporalEntrada)) {

        // 3. Construir y ejecutar el comando
        // El comando será: ./afin_cipher <alpha> <b> <E/D> <archivo_entrada>
        $comando = "./afin_cipher " . $alpha . " " . $beta . " " . $opcion . " " . $nombreTemporalEntrada;
        
        // shell_exec ejecuta el comando y nos devuelve lo que el programa C imprimió (el nombre del archivo de salida)
        $nombreArchivoSalida = shell_exec($comando);

        // Limpiar el nombre de salida por si hay espacios o saltos de línea
        $nombreArchivoSalida = trim($nombreArchivoSalida);

        // 4. Verificar si el archivo de salida se generó
        if (file_exists($nombreArchivoSalida)) {
            // Forzar la descarga del archivo generado
            header('Content-Description: File Transfer');
            header('Content-Type: application/octet-stream');
            header('Content-Disposition: attachment; filename="' . basename($nombreArchivoSalida) . '"');
            header('Expires: 0');
            header('Cache-Control: must-revalidate');
            header('Pragma: public');
            header('Content-Length: ' . filesize($nombreArchivoSalida));
            readfile($nombreArchivoSalida);

            // 5. Limpieza: borrar los archivos temporales
            unlink($nombreTemporalEntrada);
            unlink($nombreArchivoSalida);
            exit;

        } else {
            echo "Error: El programa en C no generó un archivo de salida. Posible error en el programa o en los parámetros.<br>";
            echo "Comando ejecutado: " . htmlspecialchars($comando) . "<br>";
            echo "Respuesta del programa: " . htmlspecialchars($nombreArchivoSalida);
        }

    } else {
        echo "Error: no se pudo mover el archivo subido.";
    }
} else {
    echo "Error al subir el archivo. Código de error: " . $_FILES['archivo']['error'];
}
?>