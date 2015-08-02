# Set $ALE_PATH env var should be set to ALE installation directory path
g++ -o ../build/addon ../src/ale.cpp -I $ALE_PATH/src -L$ALE_PATH -lale