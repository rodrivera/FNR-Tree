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

	int64_t id_n1, id_n2;
	u_int8_t len;
	u_int8_t* name;
	int64_t id;
	int32_t cl;

	while(true){

		fread(&id_n1, sizeof(id_n1), 1, in);
	  	id_n1 = (int64_t)be64toh( (u_int64_t)id_n1);

		fread(&id_n2, sizeof(id_n2), 1, in);
	  	id_n2 = (int64_t)be64toh( (u_int64_t)id_n2);

		fread(&len, sizeof(len),1 , in);
		name = (u_int8_t *)malloc(len);
		fread(name, sizeof(name[0]), len, in);

		fread(&id, sizeof(id), 1, in);
	  	id = (int64_t)be64toh( (u_int64_t)id);		

	  	fread(&cl, sizeof(cl), 1, in);
	  	cl = (int32_t)be32toh( (u_int32_t)cl );

/*
		cout << "id_n1 = "; hex64(id_n1);
		cout << "id_n2 = "; hex64(id_n2);
		cout << "id = "; hex64(id);
		cout << "class = "; hex32(cl);
*/
	  	if(id < 0) break;
		cout << id << " " << id_n1 << " "<< id_n2 << " " << name << endl;

	}

	return 0;
}