#include "loader.h"


//TODO: this method is too long, make it shorter by refactoring
bool Loader::loadPartition(int id, Partition &p, bool readable)
{
	static char s[32 + 1] = { '0', };
	vector<Vertex>& data=p.getData();
	data.clear();

	FILE *fp;
	char label;
	int size = 0;
	int numOfPartition;
	int src, degree, dst;
	int numVertices = 0;
	int i = 0;
	string str;
	char *ctemp[2];
	char buf[512];
	char *p_token = NULL;
	char *context = NULL;
	if (readable) {
		str = GRAP + "." + PART + "." + HUMA + "." + std::to_string((long long)id);
		fp = fopen(str.c_str(), "r");

		while (NULL != fgets(buf, sizeof(buf), fp)) {
			vector<vertexid_t> outEdges;
			vector<label_t> outEdgeValues;

			p_token = strtok_r(buf, "\n", &context);
			p_token = strtok_r(buf, "\t", &context);
			src = atoi(p_token);

			p_token = strtok_r(NULL, "\t", &context);
			degree = atoi(p_token);
			size += degree;
			numVertices++;

			p_token = strtok_r(NULL, "\t", &context);
			while (p_token != NULL) {
				ctemp[i++] = p_token;
				p_token = strtok_r(NULL, "\t", &context);

        // you don't have to do this right? you know which vector to push 
        // one vector has only elements whose index is odd, the other, even
        // that eliminate the char[2]
				if (i == 2) {
					outEdges.push_back(atoi(ctemp[0]));
					outEdgeValues.push_back(atoi(ctemp[1]));
					label = atoi(ctemp[1]);
					i = 0;
				} 
			}
			Vertex v(0, src, outEdges, outEdgeValues);
			data.push_back(v);
		}
		p.setID(id);
		p.setNumEdges(size);
		p.setNumVertices(numVertices);
		fclose(fp);
		return 1;
	}
	else {
		str = GRAP + "." + PART + "." + BINA + "." + std::to_string((long long)id);
		fp = fopen(str.c_str(), "rb");

    // wouldnt this cause problem with hard-coded value 4 and 1???
		while (0 != fread(&src, 4, 1, fp)) {
			vector<vertexid_t> outEdges;
			vector<label_t> outEdgeValues;
			fread(&degree, 4, 1, fp);
			size += degree;
			numVertices++;
			for (int i = 0; i < degree; i++) {
				fread(&dst, 4, 1, fp);

				fread(&label, 1, 1, fp);

				outEdges.push_back(dst);
				outEdgeValues.push_back(label);
			}
			Vertex v(0, src, outEdges, outEdgeValues);
			data.push_back(v);
		}
		p.setID(id);
		p.setNumEdges(size);
		p.setNumVertices(numVertices);
		fclose(fp);
		return 1;
	}
	return 0; // this is dead code!!!
}