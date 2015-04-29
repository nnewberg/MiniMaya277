#include "shaderprogram.h"
#include <la.h>
#include <iostream>


void ShaderProgram::create(const char *vertfile, const char *fragfile)
{
    prog.addShaderFromSourceFile(QOpenGLShader::Vertex  , vertfile);
    prog.addShaderFromSourceFile(QOpenGLShader::Fragment, fragfile);
    prog.link();

    // finds id in shader program, links to attr/unif variable
    attrPos = prog.attributeLocation("vs_Pos");
    attrNor = prog.attributeLocation("vs_Nor");
    attrCol = prog.attributeLocation("vs_Col");
    attrIds = prog.attributeLocation("influencer_ids");
    attrWts = prog.attributeLocation("influencer_weights");

    unifModel      = prog.uniformLocation("u_Model");
    unifModelInvTr = prog.uniformLocation("u_ModelInvTr");
    unifViewProj   = prog.uniformLocation("u_ViewProj");
    unifBindMat    = prog.uniformLocation("u_bind_matrices");
    unifJointTran  = prog.uniformLocation("u_joint_transformations");

    unifOutlined = prog.uniformLocation("u_Outlined");
    unifCentroid = prog.uniformLocation("u_Centroid");
}

void ShaderProgram::setModelMatrix(const glm::mat4 &model)
{
    prog.bind();

    if (unifModel != -1) {
        prog.setUniformValue(unifModel, la::to_qmat(model));
    }

    if (unifModelInvTr != -1) {
        glm::mat4 modelinvtr = glm::transpose(model);
        modelinvtr = glm::inverse(glm::transpose(model));
        prog.setUniformValue(unifModelInvTr, la::to_qmat(modelinvtr));
    }
}

void ShaderProgram::setViewProjMatrix(const glm::mat4& vp)
{
    prog.bind();

    if(unifViewProj != -1){
        prog.setUniformValue(unifViewProj, la::to_qmat(vp));
    }

}

//void ShaderProgram::setBindingMatrix(std::vector<glm::mat4> bms) {
//    prog.bind();
//    if (unifBindMat != -1) {
//        for (unsigned long i = 0; i < bms.size(); i++) {
//            QMatrix4x4 bm = la::to_qmat(bms.at(i));
//            prog.setUniformValueArray(unifBindMat, &bm, i);
//        }
//    }
//}

//void ShaderProgram::setJointTransform(std::vector<glm::mat4> jts) {
//    prog.bind();
//    if (unifBindMat != -1) {
//        for (unsigned long i = 0; i < jts.size(); i++) {
//            QMatrix4x4 jt = la::to_qmat(jts.at(i));
//            prog.setUniformValueArray(unifJointTran, &jt, i);
//        }
//    }
//}

void ShaderProgram::setBindingMatrix(std::vector<glm::mat4> bms) {
    prog.bind();
    if (unifBindMat != -1) {
        QMatrix4x4 * bindMats = new QMatrix4x4[bms.size()];

        for (unsigned long i = 0; i < bms.size(); i++) {
            QMatrix4x4 bm = la::to_qmat(bms.at(i));
            bindMats[i] = bm;
        }
        prog.setUniformValueArray(unifBindMat, bindMats, bms.size());
        delete [] bindMats;
    }
}

void ShaderProgram::setJointTransform(std::vector<glm::mat4> jts) {
    prog.bind();
    if (unifBindMat != -1) {
        QMatrix4x4 * jointTrans = new QMatrix4x4[jts.size()];

        for (unsigned long i = 0; i < jts.size(); i++) {
            QMatrix4x4 jt = la::to_qmat(jts.at(i));
            jointTrans[i] = jt;
        }
        prog.setUniformValueArray(unifJointTran, jointTrans, jts.size());
        delete [] jointTrans;
    }
}

void ShaderProgram::setOutlined(bool outlined){
    prog.bind();
    if (unifOutlined != -1){
        prog.setUniformValue(unifOutlined, outlined);
    }
}

void ShaderProgram::setCentroid(glm::vec4 centroid){
    prog.bind();
    if (unifCentroid != -1){
        //TODO: is QVector4D ok?
        prog.setUniformValue(unifCentroid, QVector4D(centroid[0], centroid[1],
                                                      centroid[2], 1));
    }
}



// This function, as its name implies, uses the passed in GL widget
void ShaderProgram::draw(GLWidget277 &f, Drawable &d)
{
    prog.bind();

    // Each of the following blocks checks that:
    //   * This shader has this attribute, and
    //   * This Drawable has a vertex buffer for this attribute.
    // If so, it binds the appropriate buffers to each attribute.

    if (attrPos != -1 && d.bindPos()) {
        prog.enableAttributeArray(attrPos);
        f.glVertexAttribPointer(attrPos, 4, GL_FLOAT, false, 0, NULL);
    }

    if (attrNor != -1 && d.bindNor()) {
        prog.enableAttributeArray(attrNor);
        f.glVertexAttribPointer(attrNor, 4, GL_FLOAT, false, 0, NULL);
    }

    if (attrCol != -1 && d.bindCol()) {
        prog.enableAttributeArray(attrCol);
        f.glVertexAttribPointer(attrCol, 4, GL_FLOAT, false, 0, NULL);
    }

    if (attrIds != -1 && d.bindIds()) {
        prog.enableAttributeArray(attrIds);
        f.glVertexAttribPointer(attrIds, 2, GL_FLOAT, false, 0, NULL);
    }

    if (attrWts != -1 && d.bindWts()) {
        prog.enableAttributeArray(attrWts);
        f.glVertexAttribPointer(attrWts, 2, GL_FLOAT, false, 0, NULL);
    }

    // Bind the index buffer and then draw shapes from it.
    // This invokes the shader program, which accesses the vertex buffers.
    d.bindIdx();
    f.glDrawElements(d.drawMode(), d.elemCount(), GL_UNSIGNED_INT, 0);

    if (attrPos != -1) prog.disableAttributeArray(attrPos);
    if (attrNor != -1) prog.disableAttributeArray(attrNor);
    if (attrCol!= -1) prog.disableAttributeArray(attrCol);
    if (attrIds!= -1) prog.disableAttributeArray(attrIds);
    if (attrWts!= -1) prog.disableAttributeArray(attrWts);

    f.printGLErrorLog();
}
