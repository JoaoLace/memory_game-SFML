for i in {1..5}
do
    echo "Executando ./arquivo: vez $i"
    ./death &    
    sleep 1       
done

wait  # espera todos os processos terminarem
echo "Todos os processos terminaram."
