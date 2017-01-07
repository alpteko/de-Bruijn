#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

struct node {
    string label = "STOP";
    vector<node*> links;
    vector<string> edges;
};
int main(int argc, char *argv[]){
    map<string,node*> node_dict;
    map<string,node*>::iterator it;
    int kmer_size;
    string input_file;
    vector<node>  kmer_nodes;

    input_file = argv[1];

    ifstream infile;

    infile.open(input_file);
    input_file = argv[1];
    string temp;
    infile >> temp;
    kmer_size =  stoi(temp);
    int root = 1;
    node * root_node = new node();

    while(!infile.eof())
    {
        infile >> temp;

        string suffix = temp , prefix = temp;
        suffix.erase(suffix.begin()+0);
        prefix.erase(prefix.begin()+ temp.size()-1);
        int same = 0;
        node * ptr_prefix = new node();
        node * ptr_suffix = new node();

        it = node_dict.find(suffix);

        if(suffix == prefix)
        {
            same = 1;

        }
        else if(it == node_dict.end())
        {
            ptr_suffix->label = suffix;
            node_dict.insert(node_dict.begin(),pair<string,node*>(suffix,ptr_suffix));
        }
        else
        {
            ptr_suffix = it->second;
        }

        it = node_dict.find(prefix);
        if(it == node_dict.end())
        {
            ptr_prefix->label = prefix;
            ptr_prefix->edges.push_back(temp);
            if(!same)
            {
                ptr_prefix->links.push_back(ptr_suffix);
            }
            else
            {
                ptr_prefix->links.push_back(ptr_prefix);

            }
            node_dict.insert(node_dict.begin(),pair<string,node*>(prefix,ptr_prefix));
            if(root)
            {
                root_node = ptr_prefix;
                root--;
            }

        }
        else
        {
            ptr_prefix = it->second;
            ptr_prefix->edges.push_back(temp);
            if(!same)
            {
                ptr_prefix->links.push_back(ptr_suffix);
            }
            else
            {
                ptr_prefix->links.push_back(ptr_prefix);

            }
        }
    }
    temp = root_node->edges[0];
    vector<char> output ;
    for (int i = 0; i < temp.size()-1; ++i)
    {
        output.push_back(temp[i]);
    }
    while(1)
    {
        if(root_node->links.size())
        {
            output.push_back(root_node->edges[0][kmer_size-1]);
            cout << root_node->edges[0] << endl;
            node * old_ptr = new node();
            old_ptr = root_node;
            root_node = root_node->links[0];
            old_ptr->links.erase(old_ptr->links.begin()+0);
            old_ptr->edges.erase(old_ptr->edges.begin()+0);

        }
        else
        {
            cout <<  endl << root_node->label << endl;
            break;
        }

    }

    for (int i = 0; i < output.size(); ++i)
    {
        cout << output[i];
    }

  return 0;
}