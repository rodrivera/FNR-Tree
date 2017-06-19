#include <bits/stdc++.h>
using namespace std;

void hex32(int32_t n){
	printf("%04x %04x\n",(unsigned)(n>>16),(unsigned)(n));
}

void hex64(int64_t n){
	printf("%04x %04x %04x %04x\n",(unsigned)(n>>48),(unsigned)(n>>32),(unsigned)(n>>16),(unsigned)(n));
}

int main(int argc, char const *argv[]){	
	if(argc < 2)
		return -1;
	FILE *in = fopen(argv[1],"rb");
	u_int8_t len;
	u_int8_t* name;
	int64_t id = 1;
	int32_t x, y;
	while(id >= 1){
		fread(&len, sizeof(len),1 , in);
		name = (u_int8_t *)malloc(len);
		fread(name, sizeof(name[0]), len, in);
		free(name);
		fread(&id, sizeof(id), 1, in);
	  	id = (int64_t)be64toh( (u_int64_t)id);
	  	fread(&x, sizeof(x), 1, in);
	  	x = (int32_t)be32toh( (u_int32_t)x );
	  	fread(&y, sizeof(y), 1, in);
	  	y = (int32_t)be32toh( (u_int32_t)y );
		cout << id << " "<< x << " "<< y <<endl;
	}
	return 0;
}