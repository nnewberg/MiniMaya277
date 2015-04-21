#version 150
// ^ Change this to version 130 if you have compatibility issues

// This is a vertex shader. While it is called a "shader" due to outdated conventions, this file
// is used to apply matrix transformations to the arrays of vertex data passed to it.
// Since this code is run on your GPU, each vertex is transformed simultaneously.
// If it were run on your CPU, each vertex would have to be processed in a FOR loop, one at a time.
// This simultaneous transformation allows your program to run much faster, especially when rendering
// geometry with millions of vertices.

uniform mat4 u_Model;       // The matrix that defines the transformation of the
                            // object we're rendering. In this assignment,
                            // this will be the result of traversing your scene graph.

uniform mat4 u_ModelInvTr;  // The inverse transpose of the model matrix.
                            // This allows us to transform the object's normals properly
                            // if the object has been non-uniformly scaled.

uniform mat4 u_ViewProj;    // The matrix that defines the camera's transformation.
                            // We've written a static matrix for you to use for HW2,
                            // but in HW3 you'll have to generate one yourself

uniform mat4 u_bind_matrices[100];

uniform mat4 u_joint_transformations[100];

in ivec2 influencer_ids;

in vec2 influencer_weights;

in vec4 vs_Pos;  // ---------->The array of vertex positions passed to the shader

in vec4 vs_Nor;  // ---------->The array of vertex normals passed to the shader

in vec4 vs_Col;  // ---------->The array of vertex colors passed to the shader.

out vec4 fs_Nor;  // --------->The array of normals that has been transformed by u_ModelInvTr. This is implicitly passed to the fragment shader.
out vec4 fs_LightVec;  // ---->The direction in which our virtual light lies, relative to each vertex. This is implicitly passed to the fragment shader.
out vec4 fs_Col;  // --------->The color of each vertex. This is implicitly passed to the fragment shader.

const vec4 lightPos = vec4(5, 5, 3, 1);  // The position of our virtual light, which is used to compute the shading of
                                        // the geometry in the fragment shader.

void main()
{
    fs_Col = vs_Col;  //                          Pass the vertex color positions to the fragment shader
    fs_Nor = u_ModelInvTr * vs_Nor;  //           Transform the geometry's normals

    // joint*bind*uModel
    mat4 bindMatrix0 = u_bind_matrices[influencer_ids[0]];
    mat4 bindMatrix1 = u_bind_matrices[influencer_ids[1]];

//    mat4 fullBindMatrix = bindMatrix0 + bindMatrix1;

    mat4 jointTrans0 = u_joint_transformations[influencer_ids[0]];
    mat4 jointTrans1 = u_joint_transformations[influencer_ids[1]];

//    mat4 fullJointTrans = jointTrans0 + jointTrans1;

    float normW0 = influencer_weights[0]/ (influencer_weights[0] + influencer_weights[1]);
    float normW1 = influencer_weights[1]/ (influencer_weights[0] + influencer_weights[1]);

    vec4 p0 = (jointTrans0 * bindMatrix0 * vs_Pos)*normW0;
    vec4 p1 = (jointTrans1 * bindMatrix1 * vs_Pos)*normW1;


    vec4 new_pos = p0 + p1;

//    vec4 new_pos = p0 + (influencer_weights[0]/(influencer_weights[0] + influencer_weights[1])) * (p1); //weight 0 + weight 1)) * p1 - p0

//    vec4 new_pos = fullJointTrans * fullBindMatrix * u_Model * vs_Pos;  //    Temporarily store the transformed vertex positions for use below

//    vec4 new_pos = p0 * p1;
    vec4 modelposition = u_Model * new_pos;  //    Temporarily store the transformed vertex positions for use below


    fs_LightVec = lightPos - modelposition;  //   Compute the direction in which the light source lies



    gl_Position = u_ViewProj * modelposition;  // gl_Position is a built-in variable of OpenGL which is used to render the final positions
                                             // of the geometry's vertices

    // update here for weighting
}
