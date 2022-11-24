long long int fact(long long int n){
    if (n<1) {
        return 1;
    }else {
        return n*fact(n-1);
    }
}

int main(){
    fact(3);
}
