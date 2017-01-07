#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <stack>

using namespace std;

struct node {
    string label = "STOP";
    vector<node*> links;
    vector<string> edges;
    int in = 0;
    int out = 0;
    int index =0;
    stack<int> last_used ;
    vector<int> used_egdes;

};

int main(int argc, char *argv[]){
    map<string,node*> node_dict;
    map<string,node*>::iterator it;
    int kmer_size;
    string input_file;

    input_file = argv[1];

    ifstream infile;

    infile.open(input_file);
    input_file = argv[1];
    string temp;
    infile >> temp;
    kmer_size =  stoi(temp);
    int number_of_edges = 0;
    node * root_node = new node();

    while(!infile.eof())
    {
        infile >> temp;
        number_of_edges++;
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
            ptr_prefix->in++;

        }
        else if(it == node_dict.end())
        {
            ptr_suffix->label = suffix;
            node_dict.insert(node_dict.begin(),pair<string,node*>(suffix,ptr_suffix));
            ptr_suffix->in++;

        }
        else
        {
            ptr_suffix = it->second;
            ptr_suffix->in++;

        }

        it = node_dict.find(prefix);
        if(it == node_dict.end())
        {
            ptr_prefix->label = prefix;
            ptr_prefix->edges.push_back(temp);
            if(!same)
            {
                ptr_prefix->links.push_back(ptr_suffix);
                ptr_prefix->out++;
                ptr_prefix->used_egdes.push_back(0);


            }
            else
            {
                ptr_prefix->links.push_back(ptr_prefix);
                ptr_prefix->out++;
                ptr_prefix->used_egdes.push_back(0);


            }
            node_dict.insert(node_dict.begin(),pair<string,node*>(prefix,ptr_prefix));

        }
        else
        {
            ptr_prefix = it->second;
            ptr_prefix->edges.push_back(temp);
            if(!same)
            {
                ptr_prefix->links.push_back(ptr_suffix);
                ptr_prefix->out++;
                ptr_prefix->used_egdes.push_back(0);

            }
            else
            {
                ptr_prefix->links.push_back(ptr_prefix);
                ptr_prefix->out++;
                ptr_prefix->used_egdes.push_back(0);

            }
        }
    }

    stack<node*> node_stack;
    vector<node*> node_vector;
    int index_count = 0;
    int root_found = 0;
    for(it = node_dict.begin(); it != node_dict.end(); it++)
    {
        int temp_in = it->second->in;
        int temp_out = it->second->out;
        if((temp_out-temp_in) > 0)
        {
            root_node  = it->second;
            root_found = 1;
        }
        it->second->index=index_count;
        node_vector.push_back(it->second);
        index_count++;
    }

    if(!root_found)
    {
        root_node = node_vector[0];
    }

    vector<string> output ;

    while(1)
    {
        int new_path = -1;
        for (int i = 0; i < root_node->links.size() ; ++i)
        {
            if(root_node->used_egdes[i] == 0)
            {
                new_path = i;
            }

        }
        if(output.size() == number_of_edges)
        {
            //output.push_back(root_node->label);
            break;
        }
        else if(new_path  == -1)
        {
            while(1)
            {
                root_node = node_stack.top();
                node_stack.pop();
                output.erase(output.begin()+output.size()-1);
                new_path = -1;
                for (int i = 0; i < root_node->links.size(); ++i)
                {
                    if (root_node->used_egdes[i] == 0)
                    {
                        new_path = i;
                        break;
                    }

                }
                int last_used = root_node->last_used.top();
                root_node->last_used.pop();
                root_node->used_egdes[last_used] = 0;
                if(new_path != -1)
                {
                    //output.push_back(root_node->label);
                    output.push_back(root_node->edges[new_path]);
                    root_node->last_used.push(new_path);
                    root_node->used_egdes[new_path] = 1;
                    node_stack.push(root_node);
                    root_node = root_node->links[new_path];
                    break;
                }


            }


        }
        else
        {
            //output.push_back(root_node->edges[new_path]);
            output.push_back(root_node->edges[new_path]);
            root_node->last_used.push(new_path);
            root_node->used_egdes[new_path] = 1;
            node_stack.push(root_node);
            root_node = root_node->links[new_path];

        }

    }
    string result;
    for (int j = 0; j < kmer_size-1; ++j)
    {
        result.push_back(output[0][j]);
    }

    for (int i = 0; i < output.size(); ++i)
    {
        result.push_back(output[i][kmer_size-1]);
    }
    ofstream outfile(argv[2]);
    outfile << result ;
    outfile << endl;
    outfile.close();

  return 0;
}