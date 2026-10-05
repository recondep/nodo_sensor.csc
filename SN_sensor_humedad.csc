// --- SCRIPT PARA NODOS SENSORES (SenScript) ---
loop
   read sensor_humedad x
   delay 1000
   if x < 450
      print "Alerta de estrés hídrico detectado: " x
   end
   send x 0
   delay 10000

// --- SCRIPT PARA NODO BASE SINK (SenScript) ---
loop
   wait
   read valor_recibido
   print "Trama IoT decodificada en Gateway: " valor_recibido
   store valor_recibido base_data.txt.  
