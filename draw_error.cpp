//ground_error.cpp writed by zhang ning 2018/3/15
#include <sophus/se3.h>
#include <string>
#include <iostream>
#include <fstream>

// need pangolin for plotting trajectory
#include <pangolin/pangolin.h>

using namespace std;



// function for plotting trajectory, don't edit this code
// start point is red and end point is blue
void DrawTrajectory(vector<Sophus::SE3, Eigen::aligned_allocator<Sophus::SE3>>);

int main(int argc, char **argv) {

    vector<Sophus::SE3, Eigen::aligned_allocator<Sophus::SE3>> poses1;
    vector<Sophus::SE3, Eigen::aligned_allocator<Sophus::SE3>> poses2;

    /// implement pose reading code
    // start your code here (5~10 lines)

    ifstream infile;
    infile.open("../data/groundtruth.txt");
    if(!infile) cout<<"error"<<endl;
    
    cout<<"存入数组"<<endl;   //先将文件中的数据存入到一个二维数组中
    double data;
    double a[620][8];
    double *p=&a[0][0];
    while(infile>>data)             //遇到空白符结束
    {
        *p=data;
         p++;
    }
    infile.close();

    for(int i=0;i<620;i++)   //分别对每一行数据生成一个变换矩阵，然后存入动态数组poses中
    {	
	Eigen::Quaterniond q = Eigen::Quaterniond(a[i][7],a[i][4],a[i][5],a[i][6]);
    	Eigen::Vector3d t;
	t<<a[i][1],a[i][2],a[i][3];
        Sophus::SE3 SE3_g(q,t);
        //cout<<SE3_qt<<endl;
        poses1.push_back(SE3_g);
    }
    ifstream infile1;
    infile1.open("../data/estimated.txt");
    if(!infile1) cout<<"error"<<endl;
    
    cout<<"存入数组"<<endl;   //先将文件中的数据存入到一个二维数组中
    double data1;
    double b[612][8];
    double *p1=&b[0][0];
    while(infile1>>data1)             //遇到空白符结束
    {
        *p1=data1;
         p1++;
    }
    infile1.close();

    for(int i=0;i<612;i++)   //分别对每一行数据生成一个变换矩阵，然后存入动态数组poses中
    {	
	Eigen::Quaterniond q1 = Eigen::Quaterniond(b[i][7],b[i][4],b[i][5],b[i][6]);
    	Eigen::Vector3d t1;
	t1<<b[i][1],b[i][2],b[i][3];
        Sophus::SE3 SE3_e(q1,t1);
        //cout<<SE3_qt<<endl;
        poses2.push_back(SE3_e);
    }
    // end your code here

    // draw trajectory in pangolin
    DrawTrajectory(poses1);
    DrawTrajectory(poses2);
    return 0;
}


void DrawTrajectory(vector<Sophus::SE3, Eigen::aligned_allocator<Sophus::SE3>> poses) {
    if (poses.empty()) {
        cerr << "Trajectory is empty!" << endl;
        return;
    }

    // create pangolin window and plot the trajectory
    pangolin::CreateWindowAndBind("Trajectory Viewer", 1024, 768);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    pangolin::OpenGlRenderState s_cam(
            pangolin::ProjectionMatrix(1024, 768, 500, 500, 512, 389, 0.1, 1000),
            pangolin::ModelViewLookAt(0, -0.1, -1.8, 0, 0, 0, 0.0, -1.0, 0.0)
    );

    pangolin::View &d_cam = pangolin::CreateDisplay()
            .SetBounds(0.0, 1.0, pangolin::Attach::Pix(175), 1.0, -1024.0f / 768.0f)
            .SetHandler(new pangolin::Handler3D(s_cam));


    while (pangolin::ShouldQuit() == false) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        d_cam.Activate(s_cam);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        glLineWidth(2);
        for (size_t i = 0; i < poses.size() - 1; i++) {
            glColor3f(1 - (float) i / poses.size(), 0.0f, (float) i / poses.size());
            glBegin(GL_LINES);
            auto p1 = poses[i], p2 = poses[i + 1];
            glVertex3d(p1.translation()[0], p1.translation()[1], p1.translation()[2]);
            glVertex3d(p2.translation()[0], p2.translation()[1], p2.translation()[2]);
            glEnd();
        }
        pangolin::FinishFrame();
        usleep(5000);   // sleep 5 ms
    }

}
